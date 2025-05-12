#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "display.h"
#include "audio.h"
#include "botones.h"

void setup() {
    Serial.begin(115200);
    iniciarSensor();
    iniciarDisplay();
    iniciarBotones();
    iniciarAudio();
}

void loop() {
    leerBotones();
    int distancia = leerDistancia();
    mostrarEnPantalla(distancia);
    actualizarAudio(distancia);
    delay(50);  // estabilidad
}
