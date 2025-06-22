#pragma once

#include <Arduino.h>

void iniciarBLE();
void enviarDatosBLE(int distancia, float frecuencia, int volumen, int waveform, bool isPlaying);
