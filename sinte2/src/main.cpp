#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>
#include "audio_utils.h"

#define I2S_DOUT  16
#define I2S_BCLK  17
#define I2S_LRC   18

#define SAMPLE_RATE     44100
#define TONE_FREQUENCY  440
#define AMPLITUDE       10000
#define BUFFER_SIZE     512

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

void setup() {
    Serial.begin(115200);
    iniciarAudio();
}

void loop() {
    reproducirTono();
    // Puedes quitar el delay o dejarlo muy pequeño
}