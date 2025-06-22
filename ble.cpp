#include "ble.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// UUIDs personalizados
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcdefab-1234-5678-90ab-cdefabcdefab"

BLEServer* pServer;
BLECharacteristic* pCharacteristic;

void iniciarBLE() {
    BLEDevice::init("ESP32S3_Synth");
    pServer = BLEDevice::createServer();

    BLEService* pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
    );

    pCharacteristic->addDescriptor(new BLE2902());

    pService->start();
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();

    Serial.println("🔵 BLE listo. Esperando conexión...");
}

void enviarDatosBLE(int distancia, float frecuencia, int volumen, int waveform, bool isPlaying) {
    String bleData = "Dist:" + String(distancia) + "mm, ";
    bleData += "Freq:" + String((int)frecuencia) + "Hz, ";
    bleData += "Vol:" + String(volumen) + "%, ";
    bleData += "Wave:";

    switch (waveform) {
        case 0: bleData += "Seno"; break;
        case 1: bleData += "Cuadrada"; break;
        case 2: bleData += "Triangular"; break;
    }

    bleData += ", Estado:";
    bleData += isPlaying ? "Play" : "Stop";

    pCharacteristic->setValue(bleData.c_str());
    pCharacteristic->notify();
}
