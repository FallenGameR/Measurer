#include "ParticleSensor.h"
#include <SoftwareSerial.h>
#include "../pins.h"

// http://aqicn.org/sensor/pms5003-7003/
// This study also show linear correlation between PM10 and PM2.5 reading
// It also shows likelly functional dependancy of standarf (CF1) and Atmospheric Environment (env) reading
// Looks like CF1 is normalized data that adheres to some standart, we should be using it for render
struct pms5003data
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
};

ParticleReading::ParticleReading(pms5003data *data)
{
    if (data == nullptr)
    {
        return;
    }

    this->pm10_standard = data->pm10_standard;
    this->pm25_standard = data->pm25_standard;
    this->pm100_standard = data->pm100_standard;
    this->pm10_env = data->pm10_env;
    this->pm25_env = data->pm25_env;
    this->pm100_env = data->pm100_env;
    this->particles_03um = data->particles_03um;
    this->particles_05um = data->particles_05um;
    this->particles_10um = data->particles_10um;
    this->particles_25um = data->particles_25um;
    this->particles_50um = data->particles_50um;
    this->particles_100um = data->particles_100um;
}

ParticleSensor::ParticleSensor()
{
    this->pm_sensor = new SoftwareSerial(PIN_PM_SERIAL, PIN_PM_UNUSED);
}

ParticleSensor::~ParticleSensor()
{
    delete this->pm_sensor;
}

bool ParticleSensor::ReadInternal(pms5003data *o)
{
    Stream *s = this->pm_sensor;

    if (!s->available())
    {
#ifdef DEBUG
        Serial.println("PM error: no data available");
#endif
        return false;
    }

    // Read a byte at a time until we get to the special '0x42' start-byte
    if (s->peek() != 0x42)
    {
        s->read();
#ifdef DEBUG
        Serial.println("PM error: waiting for the start byte");
#endif
        return false;
    }

    // Now read all 32 bytes
    if (s->available() < 32)
    {
#ifdef DEBUG
        Serial.println("PM error: not enough data to deserialize");
#endif
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

#ifdef DEBUG
    for (uint8_t i=2; i<32; i++) {
        Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
    }
    Serial.println();
#endif

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
#ifdef DEBUG
        Serial.println("PM error: checksum failure");
#endif
        return false;
    }

    // success!
    return true;
}

ParticleReading *ParticleSensor::Read()
{
    return this->Read(DEFAULT_READ_TIMEMOUT_MS);
}

ParticleReading *ParticleSensor::Read(uint32_t timeoutMs)
{
    pms5003data data;
    TIMEOUT_READ(Read(&data), timeoutMs);
    return new ParticleReading(&data);
}