#include "graph.h"
#include "pins.h"
#include "dht_output.h"
#include "termistor_output.h"
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

void loop()
{
  tft.fillScreen(BLACK);
  drawTermistorReading();

  PmRead();

  BmeRead();
  delay(2000);

  RtcRead();
  delay(5000);
}
