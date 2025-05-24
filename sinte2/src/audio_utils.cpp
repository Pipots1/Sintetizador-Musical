#include "audio_utils.h"
#include <math.h>

// Generar una onda cuadrada
void generarOndaCuadrada(int16_t *buffer, size_t buffer_size, float frecuencia, float amplitud, float sample_rate) {
    float periodo = sample_rate / frecuencia;
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = (i % (int)periodo < periodo / 2) ? (int16_t)amplitud : (int16_t)-amplitud;
    }
}

// Generar una onda triangular
void generarOndaTriangular(int16_t *buffer, size_t buffer_size, float frecuencia, float amplitud, float sample_rate) {
    float periodo = sample_rate / frecuencia;
    for (size_t i = 0; i < buffer_size; i++) {
        float t = fmod(i, periodo) / periodo;
        buffer[i] = (int16_t)(amplitud * (1.0f - fabs(2.0f * t - 1.0f)));
    }
}