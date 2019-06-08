#include "ParticleSensor.h"

ParticleSensor::ParticleSensor()
{
    // PM sensor baud rate is 9600
    this->pm_sensor.begin(9600);
}

void ParticleSensor::ReadInternal(ParticleReading *o)
{
    Stream *s = &this->pm_sensor;
    o->success = false;

    // Check if sensor is ready
    if (!s->available())
    {
        DEBUG_PRINT("PS error: no data available");
        return;
    }
    
    // Read a byte at a time until we get to the special '0x42' start-byte
    if (s->peek() != 0x42)
    {
        s->read();
        DEBUG_PRINT("PS error: waiting for the start byte");
        return;
    }

    // Now read all 32 bytes
    if (s->available() < 32)
    {
        DEBUG_PRINT("PS error: not enough data to deserialize");
        return;
    }

    // Read current buffer
    uint8_t buffer[32];
    s->readBytes(buffer, 32);

#ifdef DEBUG
    for (uint8_t i=2; i<32; i++) {
        Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
    }
    Serial.println();
#endif

    // The sensor comes in endian, convert it so it could be used on all platforms
    uint16_t buffer_u16[15];
    for (uint8_t i = 0; i < 15; i++)
    {
        buffer_u16[i] = buffer[2 + i*2 + 1];
        buffer_u16[i] += buffer[2 + i*2] << 8;
    }

    // Fill in the output structure
    memcpy((void *)o, (void *)buffer_u16, 30);

    // Check the checksum
    uint16_t sum = 0;
    for (uint8_t i = 0; i < 30; i++)
    {
        sum += buffer[i];
    }

    if (sum != o->checksum)
    {
        DEBUG_PRINT("PS error: checksum failure");
        return;
    }

    // Report read success
    o->success = true;
}

ParticleReading ParticleSensor::Read()
{
    TIMEOUT_READ(ReadInternal, ParticleReading, UINT32_MAX);
}

ParticleReading ParticleSensor::Read(uint32_t timeoutMs)
{
    TIMEOUT_READ(ReadInternal, ParticleReading, timeoutMs);
}