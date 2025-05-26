#include "Sensor.h"

SensorToF::SensorToF(uint8_t sda, uint8_t scl, uint8_t xshut)
    : _sda(sda), _scl(scl), _xshut(xshut) {}

bool SensorToF::begin() {
    Wire.begin(_sda, _scl);  // inicia I2C en los pines definidos

    if (_xshut != 255) {
        // Apagar el sensor
        pinMode(_xshut, OUTPUT);
        digitalWrite(_xshut, LOW);
        delay(10);

        // Encender el sensor
        digitalWrite(_xshut, HIGH);
        delay(50);  // Aumenta el delay para que el sensor arranque
    } else {
        delay(50);  // Aún esperamos un poco por seguridad
    }

    if (!lox.begin()) {
        Serial.println("❌ VL53L0X no detectado. ¿Está bien conectado?");
        return false;
    }

    Serial.println("✅ VL53L0X inicializado correctamente.");
    return true;
}


int SensorToF::leerDistancia() {
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4) {
        return measure.RangeMilliMeter;
    } else {
        return -1;  // medición no válida
    }
}