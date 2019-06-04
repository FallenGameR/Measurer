#include "hardware\termistor.h"
#include "hardware\particle.h"
//#include "hardware\bme.h"
//#include "hardware\rtc.h"

// Ctrl+Alt+U to upload
void setup(void)
{
  Serial.begin(115200);
  TftSetup();
  //PmSetup();
}



class Test
{
public:
  int value = 5;

  Test()
  {
    value = 6;
  }
};

Test function()
{
  Test result;
  result.value = 42;
  return result;
}

void loop(void)
{
  Test val = function();
  Serial.println(val.value);
  delay(3000);
  return;

  // Clear screen
  tft.fillScreen(BLACK);

  // Draw termistor reading (1s)
  //DrawTemperatureTermistor();


  DrawPmSensor();
}

/*
void setup()
{

  BmeSetup();

  RtcSetup();
}

// PROGMEM optimization notes - https://learn.adafruit.com/memories-of-an-arduino/optimizing-program-memory

void loop()
{


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
