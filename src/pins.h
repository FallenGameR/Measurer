#ifndef PINS_H
#define PINS_H

// TFT pins
#define PIN_TFT_DC 25
#define PIN_TFT_CS 24
#define PIN_TFT_MOSI 23
#define PIN_TFT_CLK 22

// Unused TFT pins
#define PIN_TFT_RST -1
#define PIN_TFT_MISO -1

// Sensor pins
#define PIN_TERMISTOR A15
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