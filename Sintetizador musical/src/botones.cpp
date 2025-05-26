// botones.cpp
#include "botones.h"

// Variables de estado externas
int modo_onda = 0;
int efecto = 0;
int modo_auto = 0;

// Crear instàncies dels botons
Boton boton1(PIN_BOTON1);
Boton boton2(PIN_BOTON2);
Boton boton3(PIN_BOTON3);

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
        Serial.print("Boton en pin ");
        Serial.print(pin);
        Serial.println(": ENCENDIDO");
        return true;
    }

    if (estadoActual == HIGH && lastState == LOW) {
        Serial.print("Boton en pin ");
        Serial.print(pin);
        Serial.println(": APAGADO");
    }

    lastState = estadoActual;
    return false;
}

void iniciarBotones() {
    boton1.iniciar();
    boton2.iniciar();
    boton3.iniciar();@
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
