#include "ParticleSensor.h"



ParticleSensor::ParticleSensor()
{
    // PM sensor baud rate is 9600
    this->pm_sensor.begin(9600);
}

bool ParticleSensor::ReadInternal(ParticleReading *o)
{
    Stream *s = &this->pm_sensor;
    o->success = false;

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
    o->success = true;
    return true;
}

ParticleReading ParticleSensor::Read()
{
    ParticleReading data;
    TIMEOUT_READ(ReadInternal(&data), data, UINT32_MAX);
    return data;

/*
    ParticleReading data;

    do
    {
        data = this->Read(DEFAULT_READ_TIMEMOUT_MS);
    } 
    while (!data.success);
    
    return data;
/**/
}

ParticleReading ParticleSensor::Read(uint32_t timeoutMs)
{
    ParticleReading data;
    TIMEOUT_READ(ReadInternal(&data), data, timeoutMs);
    return data;
}