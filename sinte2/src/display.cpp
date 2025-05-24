#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

// Classe que encapsula tota la gestió del display LCD
class DisplayLCD {
  private:
    LiquidCrystal_I2C lcd;

  public:
    DisplayLCD(uint8_t address = 0x27, uint8_t cols = 16, uint8_t rows = 2)
      : lcd(address, cols, rows) {}

    void iniciar() {
        lcd.begin(16, 2);
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print("Synth listo");
    }

    void mostrarDistancia(int distancia) {
        lcd.setCursor(0, 1);
        lcd.print("Dist: ");
        lcd.print(distancia);
        lcd.print(" mm ");
        // Afegeix espais extra per "netejar" el final si el número escurça
        lcd.print("     ");
    }

    void mostrarMissatge(const String &missatge, uint8_t fila = 0) {
        lcd.setCursor(0, fila);
        lcd.print(missatge);
        // Esborra el que sobra si el missatge és més curt que l'anterior
        int espais = 16 - missatge.length();
        for (int i = 0; i < espais; i++) {
            lcd.print(" ");
        }
    }

    void netejar() {
        lcd.clear();
    }
};

// Crear una instància global del display
DisplayLCD display;

void iniciarDisplay() {
    display.iniciar();
}

void mostrarEnPantalla(int distancia) {
    display.mostrarDistancia(distancia);
}
