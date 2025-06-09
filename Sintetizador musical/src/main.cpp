#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_VL53L0X.h>
#include <driver/i2s.h>
#include <math.h>
#include "botones.h"  

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

// === AUDIO I2S ===
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

// === AUDIO TASK ===
void taudio(void *parameter) {
  static int16_t buffer[BUFFER_SIZE];
  static float fase = 0.0f;

  while (true) {
    if (!isPlaying) {
      delay(10);
      continue;
    }

    float frecuencia = frecuenciaActual;
    float incremento = 2.0f * PI * frecuencia / SAMPLE_RATE;

    for (int i = 0; i < BUFFER_SIZE; i++) {
      float val = 0;

      switch (waveform) {
        case 0: val = sinf(fase); break; // seno
        case 1: val = fase < PI ? 1.0f : -1.0f; break; // cuadrada
        case 2: val = 2.0f * fabs(2.0f * (fase / (2.0f * PI)) - 1.0f) - 1.0f; break; // triangular
      }

      buffer[i] = (int16_t)(AMPLITUDE_MAX * (volume / 100.0f) * val);

      fase += incremento;
      if (fase > 2.0f * PI) fase -= 2.0f * PI;
    }

    size_t bytes_written;
    i2s_write(I2S_NUM_0, buffer, sizeof(buffer), &bytes_written, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

// === SENSOR + OLED TASK ===
void tsensor(void *parameter) {
  while (true) {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);

    int distancia = 0;
    int frecuencia = 440;

    if (measure.RangeStatus != 4) {
      distancia = measure.RangeMilliMeter;
      frecuencia = map(distancia, 50, 400, 100, 2000);
      frecuencia = constrain(frecuencia, 100, 2000);
      frecuenciaActual = frecuencia;
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("Distancia:");
    display.print(distancia); display.println(" mm");
    display.println("Frecuencia:");
    display.print(frecuenciaActual); display.println(" Hz");
    display.print("Volumen: "); display.println(volume);
    display.print("Onda: ");
    switch (waveform) {
      case 0: display.println("Seno"); break;
      case 1: display.println("Cuadrada"); break;
      case 2: display.println("Triangular"); break;
    }
    display.print("Estado: "); display.println(isPlaying ? "Play" : "Stop");
    display.display();

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ Error OLED");
    while (1);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Iniciando...");
  display.display();

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

  iniciarAudio();
  initBotones(); // 👈 Inicializamos botones

  xTaskCreatePinnedToCore(tsensor, "Sensor", 4096, NULL, 1, &TaskSensor, 1);
  xTaskCreatePinnedToCore(taudio,  "Audio",  4096, NULL, 1, &TaskAudio, 0);
}

void loop() {
  leerBotones(); // 👈 Lectura de botones
  delay(50);
}
