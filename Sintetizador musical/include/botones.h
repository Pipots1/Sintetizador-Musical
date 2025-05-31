// bul.h
#ifndef BOTONES_H
#define BOTONES_H

#include <Arduino.h>

// Pines de botones
#define PIN_BOTON1 38
#define PIN_BOTON2 39
#define PIN_BOTON3 40

// Variables globales
extern int modo_onda;
extern int efecto;
extern int modo_auto;

class Boton {
public:
    Boton(uint8_t pin, unsigned long debounceDelay = 200);
    void iniciar();
    bool fuePresionado();

private:
    uint8_t pin;
    unsigned long debounceDelay;
    unsigned long lastPressTime;
    bool lastState;
};

void iniciarBotones();
void leerBotones();

#endif
