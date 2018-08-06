#ifndef PINS_H
#define PINS_H

// TFT pins
#define PIN_TFT_DC 5
#define PIN_TFT_CS 4
#define PIN_TFT_MOSI 3
#define PIN_TFT_CLK 2

// Unused TFT pins
#define PIN_TFT_RST -1
#define PIN_TFT_MISO -1

// Sensor pins
#define PIN_TERMISTOR A0
#define PIN_DHT11 6

// Particle concentration sensor
#define PIN_PM_SERIAL 6
#define PIN_PM_UNUSED 3

// BME pins
#define PIN_BME_SCK 13
#define PIN_BME_MISO 12
#define PIN_BME_MOSI 11
#define PIN_BME_CS 10

#endif // PINS_H