#ifndef TERMISTOR_OUTPUT_H
#define TERMISTOR_OUTPUT_H

#include "graph.h"
#include "temperature.h"

void drawTermistorReading()
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