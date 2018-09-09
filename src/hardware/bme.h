#ifndef BME_H
#define BME_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include "..\pins.h"

// You can also calculate Altitude. However, you can only really do a good accurate job of calculating altitude if
// you know the hPa pressure at sea level for your location and day! The sensor is quite precise but if you do not
// have the data updated for the current day then it can be difficult to get more accurate than 10 meters.
#define SEALEVELPRESSURE_HPA (1013.25)

// https://www.atmos.washington.edu/~neallog/temp_real_pressure.html
// We should have 1008 for 33.5 meters elevation

Adafruit_BME680 bme(PIN_BME_CS, PIN_BME_MOSI, PIN_BME_MISO, PIN_BME_SCK);

void BmeSetup()
{
    if (!bme.begin())
    {
        Serial.println("Could not find a valid BME680 sensor, check wiring!");
        while (1)
            ;
    }

    // Set up oversampling for noise reduction

    // Temperature reading is used to correct for temperature variations of other samples
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    // Supresses short disturbances of data and
    // increases resolution of sensors
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
}

// In Celcius
double ReadTemperatureBme()
{
    return bme.temperature;
}

// 1 sec delay between readings
void BmeRead()
{
    if (!bme.performReading())
    {
        Serial.println("Failed to perform reading :(");
        return;
    }

    // At 25C accuracy 0.5C
    // Full accuracy on 0-65C range is 1C
    Serial.print("Temperature = ");
    Serial.print(bme.temperature);
    Serial.println(" *C");

    // Absolute accuracy preassure 600Pa
    // 0.12Pa RMS noise
    // 1.3Pa/K offset temperature coefficient (10.9 cm per 1 Celcius)
    // Long term stability 100Pa per year
    Serial.print("Pressure = ");
    Serial.print(bme.pressure / 100.0);
    Serial.println(" hPa");

    // ~8 seconds for response
    // 3% accuracy
    // Long term stability 0.5% per year
    Serial.print("Humidity = ");
    Serial.print(bme.humidity);
    Serial.println(" %");

    // Takes 30 min to stabilize. Then the reading is used for baseline.
    // ~1 seconds for response
    // IAQ index output 0..500. To 50 good, to 100 average, after 300 very bad
    // RMS noise of resistance 1.5%
    // 15% seonsor to sensor deviation
    // Detects: Ethane, Isoprene, Ethanol, Acetone, Carbone Monoxide, certified accuracy 5% max
    Serial.print("Gas = ");
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(" KOhms");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.println();
}

#endif // BME_H