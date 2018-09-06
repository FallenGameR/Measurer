#include "graph.h"
#include "pins.h"
#include "termistor.h"
#include "pm_sensor.h"
#include "bme_sensor.h"
#include "rtc.h"

void setup()
{
  Serial.begin(9600);

  tft.begin();
  tft.setRotation(1);

  Serial.println("ILI9341 screen");
  Serial.print("Width: ");
  Serial.println(tft.width());
  Serial.print("Height: ");
  Serial.println(tft.height());

  // PM sensor baud rate is 9600
  pm_sensor.begin(9600);

  BmeSetup();

  RtcSetup();
}

// PROGMEM optimization notes - https://learn.adafruit.com/memories-of-an-arduino/optimizing-program-memory

void loop()
{
  // All
  // DATA:     1603 /  2048 = 78.3%
  // PROGRAM: 29592 / 32256 = 91.7% (2664 left)
  tft.fillScreen(BLACK);

  // Graph termistor
  // DATA:    26
  // PROGRAM: 3628
  DrawTemperatureTermistor();

  // PM
  // DATA:    364
  // PROGRAM: 1282
  PmRead();

  // BME
  // DATA:    256
  // PROGRAM: 6458
  BmeRead();
  delay(2000);

  // RTC
  // DATA:    138
  // PROGRAM: 1182
  RtcRead();
  delay(5000);
}