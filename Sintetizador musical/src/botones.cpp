#include "config.h"
#include <Arduino.h>

int modo_onda = 0;
int efecto = 0;
int modo_auto = 0;

void iniciarBotones() {
    pinMode(PIN_BOTON1, INPUT_PULLUP);
    pinMode(PIN_BOTON2, INPUT_PULLUP);
    pinMode(PIN_BOTON3, INPUT_PULLUP);
}

void leerBotones() {
    if (!digitalRead(PIN_BOTON1)) {
        modo_onda = (modo_onda + 1) % 3;
        delay(200);  // debounce
    }
    if (!digitalRead(PIN_BOTON2)) {
        efecto = !efecto;
        delay(200);
    }
    if (!digitalRead(PIN_BOTON3)) {
        modo_auto = !modo_auto;
        delay(200);
    }
}
