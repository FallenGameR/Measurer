#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "pins.h"

// The value of resistor connected sequential with termistor in ohms
#define SERIESRESISTOR 1e4

// Nominal temperature in celcius
#define TEMPERATURENOMINAL 25

// Termistor resistance at nominal temperature
#define THERMISTORNOMINAL 1e4

// The beta coefficient of the thermistor from datasheet
#define BCOEFFICIENT 3950

double ReadTemperatureTermistor()
{
    int vi = analogRead(PIN_TERMISTOR);
    double r = SERIESRESISTOR / (1023.0 / vi - 1.0);

    double steinhart;
    steinhart = r / THERMISTORNOMINAL;                // (R/Ro)
    steinhart = log(steinhart);                       // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                        // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                      // Invert
    steinhart -= 273.15;                              // convert to C

    return steinhart;
}

#endif // TEMPERATURE_H