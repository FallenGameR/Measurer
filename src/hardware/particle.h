#ifndef PARTICLE_H
#define PARTICLE_H

#include <SoftwareSerial.h>
#include "..\pins.h"
#include "..\graph.h"

// For UNO and others without hardware serial, we must use software serial.
// First pin is IN from sensor (TX pin on sensor), leave other pin disconnected
SoftwareSerial pm_sensor(PIN_PM_SERIAL, PIN_PM_UNUSED);

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

void PmSetup()
{
    // PM sensor baud rate is 9600
    pm_sensor.begin(9600);
}

boolean ReadPmSensor(Stream *s, pms5003data *o)
{
    if (!s->available())
    {
        Serial.println("PM error: no data available");
        return false;
    }

    // Read a byte at a time until we get to the special '0x42' start-byte
    if (s->peek() != 0x42)
    {
        s->read();
        Serial.println("PM error: waiting for the start byte");
        return false;
    }

    // Now read all 32 bytes
    if (s->available() < 32)
    {
        Serial.println("PM error: not enough data to deserialize");
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
    memcpy((void *)o, (void *)buffer_u16, 30);

    if (sum != o->checksum)
    {
        Serial.println("PM error: checksum failure");
        return false;
    }

    // success!
    return true;
}

void ReadPmSensor(pms5003data *data)
{
    // Read until successful
    while (!ReadPmSensor(&pm_sensor, data))
    {
        delay(1);
    }
}

void ReadPmSensor()
{
    struct pms5003data pm_sensor_data;
    ReadPmSensor(&pm_sensor_data);

    // reading pm_sensor_data was successful!
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");

    Serial.print("PM 1.0 (ug / m^3): ");
    Serial.println(pm_sensor_data.pm10_standard);
    Serial.print("PM 2.5 (ug / m^3): ");
    Serial.println(pm_sensor_data.pm25_standard);
    Serial.print("PM 10 (ug / m^3) - likelly reapeats PM 2.5: ");
    Serial.println(pm_sensor_data.pm100_standard);

    Serial.print("Particles > 0.3 um / 0.1 L air:");
    Serial.println(pm_sensor_data.particles_03um);
    Serial.print("Particles > 0.5 um / 0.1 L air:");
    Serial.println(pm_sensor_data.particles_05um);
    Serial.print("Particles > 1.0 um / 0.1 L air:");
    Serial.println(pm_sensor_data.particles_10um);
    Serial.print("Particles > 2.5 um / 0.1 L air:");
    Serial.println(pm_sensor_data.particles_25um);
    Serial.print("Particles > 5.0 um / 0.1 L air:");
    Serial.println(pm_sensor_data.particles_50um);
    Serial.print("Particles > 50 um / 0.1 L air:");
    Serial.println(pm_sensor_data.particles_100um);
}

void DrawPmSensor()
{
    struct pms5003data data; //
    ReadPmSensor(&data);     //

    double x = 0;
    double y = data.particles_03um; //

    box screen;
    screen.xlo = 0;
    screen.ylo = 0;
    screen.xhi = tft.width() - 1;
    screen.yhi = tft.height() - 1;

    box plot;
    plot.xlo = 0;
    plot.xhi = 60;
    plot.ylo = (int)y - 300; //
    plot.yhi = (int)y + 300; //

    box line;
    line.xlo = MAP_X(x, plot, screen);
    line.ylo = MAP_Y(y, plot, screen);

    // Draw grid                          //
    InitializeGrid(tft, screen, plot, 10, 100, DKBLUE, WHITE, BLACK);
    //                                //                               //
    InitializeAxes(tft, screen, plot, "P > 0.3 um in 0.1L air", "sec", "particles", RED, WHITE, BLACK);

    // Draw graph
    for (double x = 1; x <= 60; x += 1)
    {
        ReadPmSensor(&data); //
        line.xhi = x;
        line.yhi = data.particles_03um; //
        Graph(tft, screen, plot, line, GREEN);
        delay(1000);

        //Serial.print("PM 1.0 (ug / m^3): ");
        //Serial.println(data.pm10_standard);
    }
}

#endif // PARTICLE_H