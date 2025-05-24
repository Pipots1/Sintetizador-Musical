#pragma once

// VL53L0X (sensor de distancia)
#define PIN_SDA         18    // SDA I2C
#define PIN_SCL         17    // SCL I2C
#define PIN_XSHUT       16    // Apagado/reinicio del sensor (opcional)

// LCD I2C (usa el mismo bus que el VL53L0X)
#define PIN_LCD_VCC     5     // Alimentación (puede ser 3.3V o 5V según módulo)
#define PIN_LCD_GND     0     // GND (elige GND de la placa)

// Pulsadores
#define PIN_BOTON1      40
#define PIN_BOTON2      39
#define PIN_BOTON3      38

// MAX98357A (audio I2S)
#define PIN_I2S_BCLK    37    // Bit Clock
#define PIN_I2S_LRC     36    // Word Select (LRC)
#define PIN_I2S_DIN     35    // Data In

// Opcional: alimentación
#define PIN_AUDIO_VCC   3.3   // VCC de audio
#define PIN_AUDIO_GND   0     // GND
