#include "graph.h"
#include "pins.h"
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
  // DATA:    [========  ]  78.3% (used 1603 bytes from 2048 bytes)
  // PROGRAM: [========= ] 91.7 % (used 29592 bytes from 32256 bytes)
  tft.fillScreen(BLACK);

  // DATA:    [========  ]  77.0% (used 1577 bytes from 2048 bytes)
  // PROGRAM: [========  ]  80.5% (used 25964 bytes from 32256 bytes)
  drawTermistorReading();

  // DATA:    [======    ]  60.5% (used 1239 bytes from 2048 bytes)
  // PROGRAM: [========= ]  87.8% (used 28310 bytes from 32256 bytes)
  PmRead();

  // DATA:    [=======   ]  65.8% (used 1347 bytes from 2048 bytes)
  // PROGRAM: [=======   ]  71.7% (used 23134 bytes from 32256 bytes)
  BmeRead();
  delay(2000);

  // DATA:    [=======   ]  71.5% (used 1465 bytes from 2048 bytes)
  // PROGRAM: [========= ]  88.1% (used 28410 bytes from 32256 bytes)
  RtcRead();
  delay(5000);
}
