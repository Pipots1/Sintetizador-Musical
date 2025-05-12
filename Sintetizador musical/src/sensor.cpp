#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void iniciarSensor() {
    Wire.begin(PIN_SDA, PIN_SCL);
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
