#include "config.h"
#include <Arduino.h>

// Clase para gestionar un botón con debounce
class Boton {
  private:
    uint8_t pin;
    unsigned long lastPressTime;
    unsigned long debounceDelay;
    bool lastState;

  public:
    Boton(uint8_t pin, unsigned long debounceDelay = 200)
        : pin(pin), debounceDelay(debounceDelay), lastPressTime(0), lastState(HIGH) {}

    void iniciar() {
        pinMode(pin, INPUT_PULLUP);
    }

    // Retorna true només si el botó s'ha premut (amb debounce)
    bool fuePresionado() {
        bool estadoActual = digitalRead(pin);
        unsigned long ahora = millis();

        if (estadoActual == LOW && lastState == HIGH && (ahora - lastPressTime > debounceDelay)) {
            lastPressTime = ahora;
            lastState = estadoActual;
            return true;
        }

        lastState = estadoActual;
        return false;
    }
};

// Variables globals del sistema
int modo_onda = 0;
int efecto = 0;
int modo_auto = 0;

// Crear instàncies dels 3 botons
Boton boton1(PIN_BOTON1);
Boton boton2(PIN_BOTON2);
Boton boton3(PIN_BOTON3);

void iniciarBotones() {
    boton1.iniciar();
    boton2.iniciar();
    boton3.iniciar();
}

void leerBotones() {
    if (boton1.fuePresionado()) {
        modo_onda = (modo_onda + 1) % 3;
    }

    if (boton2.fuePresionado()) {
        efecto = !efecto;
    }

    if (boton3.fuePresionado()) {
        modo_auto = !modo_auto;
    }
}
