#ifndef PARTICLE_SENSOR_H
#define PARTICLE_SENSOR_H

#include <Arduino.h>

#define TIMEOUT_READ(readOperation, timeoutMs)   \
    {                                            \
        auto started = millis();                 \
        while (!(readOperation))                 \
        {                                        \
            if (millis() - started >= timeoutMs) \
            {                                    \
                return nullptr;                  \
            }                                    \
            else                                 \
            {                                    \
                delay(1);                        \
            }                                    \
        }                                        \
    }

class SoftwareSerial;
struct pms5003data;

#define DEFAULT_READ_TIMEMOUT_MS 1000

// https://www.researchgate.net/publication/320555036_Particle_Distribution_Dependent_Inaccuracy_of_the_Plantower_PMS5003_low-cost_PM-sensor
// PM10 and more likelly can be ignored. Looks like the sensor just multiplies P2.5 reading to output P10

class ParticleReading
{
public:
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

    ParticleReading(pms5003data *data);
};


class ParticleSensor
{
private:
    SoftwareSerial *pm_sensor;
    bool ReadInternal(pms5003data *o);

public:
    ParticleSensor();
    ~ParticleSensor();
    ParticleReading *Read();
    ParticleReading *Read(uint32_t timeoutMs);
};

#endif
