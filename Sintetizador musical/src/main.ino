#include <Arduino.h>
#include "config.h"
#include "botones.h"

int modo_onda = 0;
int efecto = 0;
int modo_auto = 0;

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

void setup() {
    iniciarBotones();
    // Altres inicialitzacions com display, sensors...
}

void loop() {
    leerBotones();
    // Resta del bucle principal: actualitzar display, actuadors, etc.
    delay(10); // petit delay per evitar lectura contínua massa ràpida
}
