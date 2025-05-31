// pul.cpp
#include "pul.h"

int modo_onda = 0;
int efecto = 0;
int modo_auto = 0;

Boton boton1(PIN_BOTON1);
Boton boton2(PIN_BOTON2);
Boton boton3(PIN_BOTON3);

Boton::Boton(uint8_t pin, unsigned long debounceDelay)
  : pin(pin), debounceDelay(debounceDelay), lastPressTime(0), lastState(HIGH) {}

void Boton::iniciar() {
    pinMode(pin, INPUT_PULLUP);
    // Leer el estado inicial
    lastState = digitalRead(pin);
}

bool Boton::fuePresionado() {
    bool estadoActual = digitalRead(pin);
    unsigned long ahora = millis();
    
    // Detectar flanco descendente (botón presionado)
    if (estadoActual != lastState) {
        // Esperar a que el estado se estabilice
        delay(5); // Pequeña espera para el debounce físico
        estadoActual = digitalRead(pin);
        
        if (estadoActual == LOW && lastState == HIGH && (ahora - lastPressTime > debounceDelay)) {
            lastPressTime = ahora;
            lastState = estadoActual;
            return true;
        }
        
        lastState = estadoActual;
    }
    
    return false;
}

void iniciarBotones() {
    boton1.iniciar();
    boton2.iniciar();
    boton3.iniciar();
}

void leerBotones() {
    if (boton1.fuePresionado()) {
        modo_onda = (modo_onda + 1) % 3;
        Serial.println("Botón 1 presionado");
    }
    if (boton2.fuePresionado()) {
        efecto = !efecto;
        Serial.println("Botón 2 presionado");
    }
    if (boton3.fuePresionado()) {
        modo_auto = !modo_auto;
        Serial.println("Botón 3 presionado");
    }
}
