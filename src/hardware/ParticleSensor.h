#ifndef PARTICLE_SENSOR_H
#define PARTICLE_SENSOR_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../pins.h"

#define TIMEOUT_READ(readOperation, default_return, timeoutMs)   \
    {                                            \
        auto started = millis();                 \
        while (!(readOperation))                 \
        {                                        \
            if (millis() - started >= timeoutMs) \
            {                                    \
                return default_return;           \
            }                                    \
            else                                 \
            {                                    \
                delay(1);                        \
            }                                    \
        }                                        \
    }

#ifdef DEBUG
    #define DEBUG_PRINT(message) Serial.println(F(message));
#else
    #define DEBUG_PRINT(message) ;
#endif


#define DEFAULT_READ_TIMEMOUT_MS 1000

// https://www.researchgate.net/publication/320555036_Particle_Distribution_Dependent_Inaccuracy_of_the_Plantower_PMS5003_low-cost_PM-sensor
// PM10 and more likelly can be ignored. Looks like the sensor just multiplies P2.5 reading to output P10

// http://aqicn.org/sensor/pms5003-7003/
// This study also show linear correlation between PM10 and PM2.5 reading
// It also shows likelly functional dependancy of standarf (CF1) and Atmospheric Environment (env) reading
// Looks like CF1 is normalized data that adheres to some standart, we should be using it for render
struct ParticleReading
{
    uint16_t framelen;
    uint16_t pm10_standard;
    uint16_t pm25_standard;
    uint16_t pm100_standard;
    uint16_t pm10_env;
    uint16_t pm25_env;
    uint16_t pm100_env;
    uint16_t particles_03um;
    uint16_t particles_05um;
    uint16_t particles_10um;
    uint16_t particles_25um;
    uint16_t particles_50um;
    uint16_t particles_100um;
    uint16_t unused;
    uint16_t checksum;
    bool success;
};

class ParticleSensor
{
private:
    // For UNO and others without hardware serial, we must use software serial.
    // First pin is IN from sensor (TX pin on sensor), leave other pin disconnected.
    SoftwareSerial pm_sensor = SoftwareSerial(PIN_PM_SERIAL, PIN_PM_UNUSED);
    bool ReadInternal(ParticleReading *o);

public:
    ParticleSensor();
    ParticleReading Read();
    ParticleReading Read(uint32_t timeoutMs);
};

#endif
