#ifndef BOTONES_H
#define BOTONES_H

#include <Arduino.h>

class Boton {
  private:
    uint8_t pin;
    unsigned long lastPressTime;
    unsigned long debounceDelay;
    bool lastState;

  public:
    Boton(uint8_t pin, unsigned long debounceDelay = 200);
    void iniciar();
    bool fuePresionado();
};

#endif