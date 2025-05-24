#pragma once
#include <Arduino.h>

// Función para generar una onda cuadrada
void generarOndaCuadrada(int16_t *buffer, size_t buffer_size, float frecuencia, float amplitud, float sample_rate);

// Función para generar una onda triangular
void generarOndaTriangular(int16_t *buffer, size_t buffer_size, float frecuencia, float amplitud, float sample_rate);