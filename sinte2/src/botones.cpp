#include "botones.h"
#include <iostream>

Boton::Boton(uint8_t pin, unsigned long debounceDelay)
  : pin(pin), debounceDelay(debounceDelay), lastPressTime(0), lastState(HIGH) {}

void Boton::iniciar() {
    pinMode(pin, INPUT_PULLUP);
}

bool Boton::fuePresionado() {
    bool estadoActual = digitalRead(pin);
    unsigned long ahora = millis();

    if (estadoActual == LOW && lastState == HIGH && (ahora - lastPressTime > debounceDelay)) {
        lastPressTime = ahora;
        lastState = estadoActual;
        std::cout << "Boton en pin " << (int)pin << ": ENCENDIDO" << std::endl;
        return true;
    }

    if (estadoActual == HIGH && lastState == LOW) {
        std::cout << "Boton en pin " << (int)pin << ": APAGADO" << std::endl;
    }

    lastState = estadoActual;
    return false;
}
