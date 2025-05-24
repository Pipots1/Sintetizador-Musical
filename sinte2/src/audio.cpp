#include "audio.h"
#include <driver/i2s.h>
#include <math.h>

Audio::Audio(int lrcPin, int bclkPin, int dinPin)
    : _lrcPin(lrcPin), _bclkPin(bclkPin), _dinPin(dinPin), _fase(0.0) {}

void Audio::iniciar() {
    // Configuración de I2S
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = _bclkPin,
        .ws_io_num = _lrcPin,
        .data_out_num = _dinPin,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    // Instalar y configurar I2S
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void Audio::generarOndaSenoidal(float frecuencia, float amplitud) {
    const int samples = 128; // Número de muestras por bloque
    int16_t buffer[samples * 2]; // Estéreo: 2 canales
    float incrementoFase = 2.0f * PI * frecuencia / 44100.0f;

    for (int i = 0; i < samples; i++) {
        int16_t muestra = (int16_t)(amplitud * sinf(_fase));
        _fase += incrementoFase;
        if (_fase > 2.0f * PI) _fase -= 2.0f * PI;

        buffer[i * 2] = muestra;     // Canal izquierdo
        buffer[i * 2 + 1] = muestra; // Canal derecho
    }

    size_t bytesEscritos;
    i2s_write(I2S_NUM_0, buffer, sizeof(buffer), &bytesEscritos, portMAX_DELAY);
}
