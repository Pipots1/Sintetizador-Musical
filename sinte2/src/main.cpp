#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>
#include "audio_utils.h"  // Incluir las utilidades de audio

#define I2S_DOUT  16  // DIN del MAX98357A
#define I2S_BCLK  17  // Bit Clock
#define I2S_LRC   18  // LRC/WS (Word Select)

// Parámetros de la señal
#define SAMPLE_RATE     44100
#define TONE_FREQUENCY  440    // A4 (440 Hz)
#define AMPLITUDE       10000  // Amplitud de la señal senoidal
#define BUFFER_SIZE     512

void iniciarAudio() {
    // Configuración I2S para salida en modo maestro
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

    // Instanciar el periférico I2S
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_sample_rates(I2S_NUM_0, SAMPLE_RATE);
}

void reproducirTono() {
    static int16_t buffer[BUFFER_SIZE];

    // Generar una onda senoidal
    for (int i = 0; i < BUFFER_SIZE; i++) {
        float t = (float)i / SAMPLE_RATE;
        buffer[i] = (int16_t)(AMPLITUDE * sin(2.0 * PI * TONE_FREQUENCY * t));
    }

    size_t bytes_written;
    i2s_write(I2S_NUM_0, buffer, sizeof(buffer), &bytes_written, portMAX_DELAY);
}

void setup() {
    Serial.begin(115200);
    iniciarAudio();
}

void loop() {
    reproducirTono();  // Envía bloques continuamente
    delay(10);         // Pequeño retraso para evitar saturar el bucle
}