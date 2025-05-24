#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

// Define los pines SDA y SCL
#define PIN_SDA 8  // Cambia por el pin correcto para SDA
#define PIN_SCL 9  // Cambia por el pin correcto para SCL

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void iniciarSensor() {
    Wire.begin(PIN_SDA, PIN_SCL);
    Serial.println("Sensor inicializado");
    if (!lox.begin()) {
        Serial.println("¡Error inicializando VL53L0X!");
        while (1);
    }
}

int leerDistancia() {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) return measure.RangeMilliMeter;
    return -1;  // error
}
