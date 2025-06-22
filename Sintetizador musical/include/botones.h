#ifndef BOTONES_H
#define BOTONES_H

#include <Arduino.h>

// Pines de botones
#define BTN_PLAY     4 //(PLAY)
#define BTN_VOL_UP   5 //(BAJAR)
#define BTN_VOL_DOWN 6 //(SUBIR)
#define BTN_WAVE     7 //(TIPO ONDA)

// Variables de estado (externas)
extern bool isPlaying;
extern int volume;       // 0–100
extern int waveform;     // 0=seno, 1=cuadrada, 2=triangular

// Inicializa los pines de los botones
void initBotones();

// Debe llamarse en loop para gestionar las pulsaciones
void leerBotones();

#endif
