#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "display.h"

// Dirección I2C habitual (ajusta con escáner si no funciona)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // o prueba con 0x3F si no se ve nada

void iniciarDisplay() {
    Serial.println("Iniciant display...");

    Wire.begin(18, 17);  // SDA = GPIO18, SCL (SCK) = GPIO17
    lcd.init();          // Importante usar lcd.init() en lugar de lcd.begin()
    lcd.backlight();     // Enciende la luz de fondo

    lcd.setCursor(0, 0);
    lcd.print("Synth llest");
}

void mostrarEnPantalla(int distancia) {
    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    lcd.print(distancia);
    lcd.print(" mm   ");  // Espais extra per netejar línia
}