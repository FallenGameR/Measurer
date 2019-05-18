#include "hardware\bme.h"
#include "hardware\particle.h"
#include "hardware\rtc.h"
#include "hardware\termistor.h"

// Ctrl+Alt+U to upload
void setup(void)
{
  Serial.begin(9600);
  TftSetup();

}

void loop(void)
{
  // Clear screen
  tft.fillScreen(BLACK);

  // Graph termistor - makes screen white
  DrawTemperatureTermistor();
  delay(1000);
}

/*
void setup()
{

  // PM sensor baud rate is 9600
  pm_sensor.begin(9600);

  BmeSetup();

  RtcSetup();
}

// PROGMEM optimization notes - https://learn.adafruit.com/memories-of-an-arduino/optimizing-program-memory

void loop()
{

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

}
/**/
