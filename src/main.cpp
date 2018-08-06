#include "graph.h"
#include "pins.h"
#include "dht_output.h"
#include "termistor_output.h"
#include "pm_sensor.h"

void setup()
{
  Serial.begin(9600);

  // sensor baud rate is 9600
  pm_sensor.begin(9600);
}

void loop()
{
  if (ReadPmSensor(&pm_sensor))
  {
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
}

/*
// BME
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME680 bme; // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println(F("BME680 test"));

  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop()
{
  if (!bme.performReading())
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(2000);
}
*/

/* RTC

#include <Wire.h>
#include "RTClib.h"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{

#ifndef ESP8266
  while (!Serial)
    ; // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop()
{
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");

  // calculate a date which is 7 days and 30 seconds into the future
  DateTime future(now + TimeSpan(7, 12, 30, 6));

  Serial.print(" now + 7d + 30s: ");
  Serial.print(future.year(), DEC);
  Serial.print('/');
  Serial.print(future.month(), DEC);
  Serial.print('/');
  Serial.print(future.day(), DEC);
  Serial.print(' ');
  Serial.print(future.hour(), DEC);
  Serial.print(':');
  Serial.print(future.minute(), DEC);
  Serial.print(':');
  Serial.print(future.second(), DEC);
  Serial.println();

  Serial.println();
  delay(3000);
}
*/

/*
// Termistor

void setup()
{
  Serial.begin(9600);

  tft.begin();
  tft.setRotation(1);

  Serial.println("ILI9341 screen");
  Serial.print("Width: ");
  Serial.println(tft.width());
  Serial.print("Height: ");
  Serial.println(tft.height());
}

void loop(void)
{
  tft.fillScreen(BLACK);
  drawTermistorReading();
}
*/