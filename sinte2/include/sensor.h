#pragma once
#include <Adafruit_VL53L0X.h>
#include <Wire.h>

class SensorToF {
public:
    SensorToF(uint8_t sda, uint8_t scl, uint8_t xshut = 255);  // constructor
    bool begin();
    int leerDistancia();  // devuelve distancia en mm

private:
    uint8_t _sda, _scl, _xshut;
    Adafruit_VL53L0X lox;
};