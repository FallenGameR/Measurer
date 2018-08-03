#include "graph.h"
#include "pins.h"
#include "dht_output.h"
#include "termistor_output.h"

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
}

void loop(void)
{
  tft.fillScreen(BLACK);
  drawTermistorReading();
  delay(60000);
}
