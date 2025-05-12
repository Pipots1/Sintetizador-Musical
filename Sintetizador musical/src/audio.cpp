#include <Arduino.h>
#include "config.h"

void iniciarAudio() {
    ledcSetup(0, 1000, 8);  // canal 0, freq 1kHz, 8-bit
    ledcAttachPin(PIN_AUDIO, 0);
}

void actualizarAudio(int distancia) {
    int freq = map(distancia, 50, 300, 100, 1000);
    ledcWriteTone(0, freq);
}
