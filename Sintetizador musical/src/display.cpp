#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void iniciarDisplay() {
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Synth listo");
}

void mostrarEnPantalla(int distancia) {
    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    lcd.print(distancia);
    lcd.print(" mm ");
}
