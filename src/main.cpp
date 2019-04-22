#include "hardware\bme.h"
#include "hardware\particle.h"
#include "hardware\rtc.h"
#include "hardware\termistor.h"

// Ctrl+Alt+U to upload
void setup()
{
  Serial.begin(9600);

  tft.begin();
  tft.setRotation(1);

  Serial.println("Screen properties");
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

  // Graph termistor - makes screen white
  // DATA:    26
  // PROGRAM: 3628
  //DrawTemperatureTermistor();

  // PM
  // DATA:    364
  // PROGRAM: 1282
  //DrawPmSensor();  // does nothing
  //ReadPmSensor();  // hangs even output from the setup
  //delay(1000);

  // BME - works
  // DATA:    256
  // PROGRAM: 6458
  //BmeRead();
  //delay(2000);

  // RTC - works but needs manual sync
  // DATA:    138
  // PROGRAM: 1182
  //RtcRead();
  //delay(5000);
}