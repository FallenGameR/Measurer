#ifndef TERMISTOR_OUTPUT_H
#define TERMISTOR_OUTPUT_H

#include "pins.h"
#include "graph.h"

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

void DrawTemperatureTermistor()
{
    double x = 0;
    double y = ReadTemperatureTermistor();

    box screen;
    screen.xlo = 0;
    screen.ylo = 0;
    screen.xhi = tft.width() - 1;
    screen.yhi = tft.height() - 1;

    box plot;
    plot.xlo = 0;
    plot.xhi = 60;
    plot.ylo = (int)y - 5;
    plot.yhi = (int)y + 5;

    box line;
    line.xlo = MAP_X(x, plot, screen);
    line.ylo = MAP_Y(y, plot, screen);

    // Draw grid
    InitializeGrid(tft, screen, plot, 10, 1, DKBLUE, WHITE, BLACK);
    InitializeAxes(tft, screen, plot, "Temparature", "sec", "C", RED, WHITE, BLACK);

    // Draw graph
    for (double x = 1; x <= 60; x += 1)
    {
        line.xhi = x;
        line.yhi = ReadTemperatureTermistor();
        Graph(tft, screen, plot, line, GREEN);
        delay(250);
    }
}

#endif // TERMISTOR_OUTPUT_H