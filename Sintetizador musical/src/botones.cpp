#include "botones.h"

// Estados anteriores para detección de flancos
static bool prevPlay = HIGH;
static bool prevUp   = HIGH;
static bool prevDown = HIGH;
static bool prevWave = HIGH;

// Variables compartidas (declaradas externamente)
bool isPlaying = true;
int volume = 100;
int waveform = 0;

void initBotones() {
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_VOL_UP, INPUT_PULLUP);
  pinMode(BTN_VOL_DOWN, INPUT_PULLUP);
  pinMode(BTN_WAVE, INPUT_PULLUP);
}

void leerBotones() {
  bool readPlay = digitalRead(BTN_PLAY);
  bool readUp   = digitalRead(BTN_VOL_UP);
  bool readDown = digitalRead(BTN_VOL_DOWN);
  bool readWave = digitalRead(BTN_WAVE);

  if (readPlay == LOW && prevPlay == HIGH) {
    isPlaying = !isPlaying;
    Serial.println(isPlaying ? "▶ Play" : "⏹ Stop");
  }

  if (readUp == LOW && prevUp == HIGH) {
    volume = min(volume + 10, 100);
    Serial.print("Volumen: "); Serial.println(volume);
  }

  if (readDown == LOW && prevDown == HIGH) {
    volume = max(volume - 10, 0);
    Serial.print("Volumen: "); Serial.println(volume);
  }

  if (readWave == LOW && prevWave == HIGH) {
    waveform = (waveform + 1) % 3;
    Serial.print("Onda actual: ");
    switch (waveform) {
      case 0: Serial.println("Seno"); break;
      case 1: Serial.println("Cuadrada"); break;
      case 2: Serial.println("Triangular"); break;
    }
  }

  prevPlay = readPlay;
  prevUp = readUp;
  prevDown = readDown;
  prevWave = readWave;
}
