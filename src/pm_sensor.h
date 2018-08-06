#ifndef PM_SENSOR_H
#define PM_SENSOR_H

#include <SoftwareSerial.h>
#include "pins.h"

// https://www.researchgate.net/publication/320555036_Particle_Distribution_Dependent_Inaccuracy_of_the_Plantower_PMS5003_low-cost_PM-sensor
// PM10 and more likelly can be ignored. Looks like the sensor just multiplies P2.5 reading to output P10

// http://aqicn.org/sensor/pms5003-7003/
// This study also show linear correlation between PM10 and PM2.5 reading
// It also shows likelly functional dependancy of standarf (CF1) and Atmospheric Environment (env) reading
// Looks like CF1 is normalized data that adheres to some standart, we should be using it for render

struct pms5003data
{
    uint16_t framelen;
    uint16_t pm10_standard, pm25_standard, pm100_standard;
    uint16_t pm10_env, pm25_env, pm100_env;
    uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
    uint16_t unused;
    uint16_t checksum;
};

// For UNO and others without hardware serial, we must use software serial.
// First pin is IN from sensor (TX pin on sensor), leave other pin disconnected
SoftwareSerial pm_sensor(PIN_PM_SERIAL, PIN_PM_UNUSED);
struct pms5003data pm_sensor_data;

boolean ReadPmSensor(Stream *s)
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

    // The pm_sensor_data comes in endian'd, this solves it so it works on all platforms
    uint16_t buffer_u16[15];
    for (uint8_t i = 0; i < 15; i++)
    {
        buffer_u16[i] = buffer[2 + i * 2 + 1];
        buffer_u16[i] += (buffer[2 + i * 2] << 8);
    }

    // put it into a nice struct :)
    memcpy((void *)&pm_sensor_data, (void *)buffer_u16, 30);

    if (sum != pm_sensor_data.checksum)
    {
        Serial.println("Checksum failure");
        return false;
    }

    // success!
    return true;
}

void PmRead()
{
    if (!ReadPmSensor(&pm_sensor))
    {
        return;
    }

    // reading pm_sensor_data was successful!
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: ");
    Serial.print(pm_sensor_data.pm10_standard);
    Serial.print("\t\tPM 2.5: ");
    Serial.print(pm_sensor_data.pm25_standard);
    Serial.print("\t\tPM 10: ");
    Serial.println(pm_sensor_data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: ");
    Serial.print(pm_sensor_data.pm10_env);
    Serial.print("\t\tPM 2.5: ");
    Serial.print(pm_sensor_data.pm25_env);
    Serial.print("\t\tPM 10: ");
    Serial.println(pm_sensor_data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:");
    Serial.println(pm_sensor_data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:");
    Serial.println(pm_sensor_data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:");
    Serial.println(pm_sensor_data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:");
    Serial.println(pm_sensor_data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:");
    Serial.println(pm_sensor_data.particles_50um);
    Serial.print("Particles > 50 um / 0.1L air:");
    Serial.println(pm_sensor_data.particles_100um);
    Serial.println("---------------------------------------");
}

#endif // PM_SENSOR_H