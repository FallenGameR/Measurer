#ifndef PINS_H
#define PINS_H

/// TFT pins
// For the Arduino Mega, use digital pins 22 through 29
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).

// Chip select
#define PIN_TFT_CS A3
// Command/Data
#define PIN_TFT_CD A2
// LCD Write
#define PIN_TFT_WR A1
// LCD Read
#define PIN_TFT_RD A0
// Reset (unused)
#define PIN_TFT_RST 0

/// Termistor pin
#define PIN_TERMISTOR A15

// Sensor pins
#define PIN_DHT11 6

// Particle concentration sensor
#define PIN_PM_SERIAL 39
#define PIN_PM_UNUSED -1

// BME pins
#define PIN_BME_SCK 34
// SDO
#define PIN_BME_MISO 35
// SDI
#define PIN_BME_MOSI 32
#define PIN_BME_CS 33

#endif // PINS_H