#ifndef PM_SENSOR_H
#define PM_SENSOR_H

#include <SoftwareSerial.h>
#include "pins.h"

// For UNO and others without hardware serial, we must use software serial.
// First pin is IN from sensor (TX pin on sensor), leave other pin disconnected
SoftwareSerial pm_sensor(PIN_PM_SERIAL, PIN_PM_UNUSED);

struct pms5003data
{
    uint16_t framelen;
    uint16_t pm10_standard, pm25_standard, pm100_standard;
    uint16_t pm10_env, pm25_env, pm100_env;
    uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
    uint16_t unused;
    uint16_t checksum;
};

struct pms5003data data;

void setup()
{
    // our debugging output
    Serial.begin(9600);

    // sensor baud rate is 9600
    pm_sensor.begin(9600);
}

boolean readPMSdata(Stream *s)
{
    if (!s->available())
    {
        return false;
    }

    // Read a byte at a time until we get to the special '0x42' start-byte
    if (s->peek() != 0x42)
    {
        s->read();
        return false;
    }

    // Now read all 32 bytes
    if (s->available() < 32)
    {
        return false;
    }

    uint8_t buffer[32];
    uint16_t sum = 0;
    s->readBytes(buffer, 32);

    // get checksum ready
    for (uint8_t i = 0; i < 30; i++)
    {
        sum += buffer[i];
    }

    /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */

    // The data comes in endian'd, this solves it so it works on all platforms
    uint16_t buffer_u16[15];
    for (uint8_t i = 0; i < 15; i++)
    {
        buffer_u16[i] = buffer[2 + i * 2 + 1];
        buffer_u16[i] += (buffer[2 + i * 2] << 8);
    }

    // put it into a nice struct :)
    memcpy((void *)&data, (void *)buffer_u16, 30);

    if (sum != data.checksum)
    {
        Serial.println("Checksum failure");
        return false;
    }
    // success!
    return true;
}

#endif PM_SENSOR_H