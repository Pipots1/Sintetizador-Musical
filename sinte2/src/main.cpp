#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>
#include "audio_utils.h"
#include "Sensor.h"  // Asegúrate de que el archivo y la clase se llaman así

// Pines de audio
#define I2S_DOUT  35
#define I2S_BCLK  37
#define I2S_LRC   36

#define SAMPLE_RATE     44100
#define TONE_FREQUENCY  440
#define AMPLITUDE       10000
#define BUFFER_SIZE     512

// Pines de conexión del sensor
#define PIN_SDA   18
#define PIN_SCL   17
#define PIN_XSHUT 16

TaskHandle_t TaskSensor;

void iniciarAudio() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_sample_rates(I2S_NUM_0, SAMPLE_RATE);
}

void reproducirTono() {
    static int16_t buffer[BUFFER_SIZE];
    static float fase = 0.0f;
    float incremento = 2.0f * PI * TONE_FREQUENCY / SAMPLE_RATE;

    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = (int16_t)(AMPLITUDE * sinf(fase));
        fase += incremento;
        if (fase >= 2.0f * PI) fase -= 2.0f * PI;
    }

    size_t bytes_written;
    i2s_write(I2S_NUM_0, buffer, sizeof(buffer), &bytes_written, portMAX_DELAY);
}

// Tarea para el sensor ToF
void tsensor(void *parameter) {
  Serial.println("🟡 Iniciando tarea tsensor...");

  SensorToF sensor(PIN_SDA, PIN_SCL, PIN_XSHUT);

  if (!sensor.begin()) {
    Serial.println("❌ Error: no se pudo inicializar el VL53L0X.");
    vTaskDelete(NULL);  // Termina la tarea si falla
  }

  Serial.println("✅ Sensor VL53L0X inicializado correctamente.");

  while (true) {
    int distancia = sensor.leerDistancia();

    if (distancia > 0) {
      Serial.print("📏 Distancia (RTOS): ");
      Serial.print(distancia);
      Serial.println(" mm");
    } else {
      Serial.println("❗ Medición no válida (RTOS)");
    }

    vTaskDelay(pdMS_TO_TICKS(500));  // espera 500 ms
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);  // esperar a que el puerto serie esté listo

  iniciarAudio();

  xTaskCreatePinnedToCore(
    tsensor,          // Función de la tarea
    "TaskSensor",     // Nombre de la tarea
    4096,             // Tamaño de la pila
    NULL,             // Parámetros para la tarea
    1,                // Prioridad
    &TaskSensor,      // Handle
    1                 // Núcleo donde correrá (0 o 1)
  );
}

void loop() {
  reproducirTono();
}