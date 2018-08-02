#ifndef DHT_OUTPUT_H
#define DHT_OUTPUT_H

#include <SimpleDHT.h>
#include "graph.h"

SimpleDHT11 dht11;

byte temperature = 0;
byte humidity = 0;

void printReadings()
{
    tft.setCursor(260, 0);
    tft.print((int)temperature);
    tft.println(" C");

    tft.setCursor(260, 8 * 2);
    tft.print((int)humidity);
    tft.println(" %");
}

void printDhtReadings()
{
    tft.setTextColor(WHITE);
    tft.setTextSize(2);

    byte new_temperature = 0;
    byte new_humidity = 0;
    byte data[40] = {0};
    int err = dht11.read(PIN_DHT11, &new_temperature, &new_humidity, data);
    if (err)
    {
        Serial.print("Read DHT11 failed, error: ");
        Serial.println(err);
        return;
    }

    bool needToErase = ((temperature != new_temperature) || (humidity != new_humidity));
    needToErase = true;

    if (needToErase)
    {
        Serial.println("erase");

        tft.setTextColor(ILI9341_BLACK);
        printReadings();

        temperature = new_temperature;
        humidity = new_humidity;

        tft.setTextColor(ILI9341_WHITE);
        printReadings();
    }
    else
    {
        Serial.println("the same");
    }
}

#endif // DHT_OUTPUT_H