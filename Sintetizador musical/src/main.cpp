#define MODO_SIMULACION false  // ← cambia a false cuando tengas el hardware real

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_VL53L0X.h>
#include <driver/i2s.h>
#include <math.h>
#include "botones.h"
#include "ble.h" //APP

// === OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === I2C ===
#define SDA_PIN 18
#define SCL_PIN 17
#define XSHUT_PIN 16

// === VL53L0X ===
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// === I2S AUDIO ===
#define I2S_DOUT 35
#define I2S_BCLK 37
#define I2S_LRC  36
#define SAMPLE_RATE 44100
#define AMPLITUDE_MAX 30000
#define BUFFER_SIZE 512

volatile float frecuenciaActual = 440.0;
TaskHandle_t TaskSensor, TaskAudio;

void iniciarAudio() {
  i2s_config_t config = {
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

  i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_sample_rates(I2S_NUM_0, SAMPLE_RATE);
}

void taudio(void *parameter) {
  static int16_t buffer[BUFFER_SIZE];
  static float fase = 0.0f;

  while (true) {
    if (!isPlaying) {
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }

    float frecuencia = frecuenciaActual;
    float incremento = 2.0f * PI * frecuencia / SAMPLE_RATE;

    for (int i = 0; i < BUFFER_SIZE; i++) {
      float val = 0;
      float factor = 1.0f;  // factor de ajuste por forma de onda

      switch (waveform) {
        case 0:  // seno
          val = sinf(fase);
          factor = 1.0f;
          break;

        case 1:  // cuadrada
          val = fase < PI ? 1.0f : -1.0f;
          factor = 0.15f;  // bajamos mucho la cuadrada
          break;

        case 2:  // triangular
          val = 2.0f * fabs(2.0f * (fase / (2.0f * PI)) - 1.0f) - 1.0f;
          factor = 0.7f;
          break;
      }

      buffer[i] = (int16_t)(AMPLITUDE_MAX * (volume / 100.0f) * val * factor);
      fase += incremento;
      if (fase > 2.0f * PI) fase -= 2.0f * PI;
    }

    size_t bytes_written;
    i2s_write(I2S_NUM_0, buffer, sizeof(buffer), &bytes_written, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(0));  // sonido más fluido
  }
}

void tsensor(void *parameter) {
  int fakeDist = 100;
  while (true) {
    int distancia = 0;
    int frecuencia = 440;

    if (MODO_SIMULACION) {
      fakeDist += 10;
      if (fakeDist > 400) fakeDist = 100;
      distancia = fakeDist;
      frecuencia = map(distancia, 50, 400, 100, 2000);
    } else {
      VL53L0X_RangingMeasurementData_t measure;
      lox.rangingTest(&measure, false);
      if (measure.RangeStatus != 4) {
        distancia = measure.RangeMilliMeter;
        frecuencia = map(distancia, 50, 400, 100, 2000);
      }
    }

    frecuencia = constrain(frecuencia, 100, 2000);
    frecuenciaActual = frecuencia;

    // Mostrar en OLED si está disponible
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.print("Dist: "); display.print(distancia); display.println(" mm");
    display.print("Freq: "); display.print(frecuencia); display.println(" Hz");
    display.print("Vol: "); display.println(volume);
    display.print("Onda: ");
    display.println(waveform == 0 ? "Seno" : waveform == 1 ? "Cuadrada" : "Triangular");
    display.print("Estado: "); display.println(isPlaying ? "Play" : "Stop");
    display.display();

    enviarDatosBLE(distancia, frecuencia, volume, waveform, isPlaying);
    vTaskDelay(pdMS_TO_TICKS(50)); //APP
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  iniciarBLE(); //APP
  Serial.println("✔ BLE iniciado correctamente");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ Error OLED");
    while (1);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Iniciando...");
  display.display();

  if (!MODO_SIMULACION) {
    pinMode(XSHUT_PIN, OUTPUT);
    digitalWrite(XSHUT_PIN, LOW); delay(10);
    digitalWrite(XSHUT_PIN, HIGH); delay(50);

    if (!lox.begin()) {
      Serial.println("❌ Error VL53L0X");
      display.clearDisplay();
      display.println("Error VL53L0X");
      display.display();
      while (1);
    }
  }

  iniciarAudio();
  initBotones();

  xTaskCreatePinnedToCore(tsensor, "Sensor", 4096, NULL, 1, &TaskSensor, 1);
  xTaskCreatePinnedToCore(taudio,  "Audio",  4096, NULL, 1, &TaskAudio, 0);
}

void loop() {
  leerBotones();
  delay(50);
}