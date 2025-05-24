#pragma once
#include <Arduino.h>

class Audio {
public:
    Audio(int lrcPin, int bclkPin, int dinPin);
    void iniciar();
    void generarOndaSenoidal(float frecuencia, float amplitud);

private:
    int _lrcPin;
    int _bclkPin;
    int _dinPin;
    float _fase;
};
