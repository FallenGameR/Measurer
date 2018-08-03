#ifndef TERMISTOR_OUTPUT_H
#define TERMISTOR_OUTPUT_H

#include "dht_output.h"
#include "graph.h"
#include "temperature.h"

void drawTermistorReading()
{
    box screen;
    screen.xlo = 0;
    screen.ylo = 0;
    screen.xhi = tft.width() - 1;
    screen.yhi = tft.height() - 1;

    box plot;
    plot.xlo = 0;  // xlo = lower bound of x axis
    plot.xhi = 60; // xhi = upper bound of x asis
    plot.ylo = 20; // ylo = lower bound of y axis
    plot.yhi = 30; // yhi = upper bound of y asis

    // Draw grid
    InitializeGrid(tft, screen, plot, 10, 1, DKBLUE, WHITE, BLACK);
    InitializeAxes(tft, screen, plot, "Temparature", "sec", "C", RED, WHITE, BLACK);

    // This transform funcition is the same as the map function, except the map uses long and we use doubles
    //double ox = MAP_X(x, plot, screen);
    //double oy = MAP_Y(y, plot, screen);

    for (double x = 0; x <= 60; x += 1)
    {
        if ((int)x % 5 == 0)
        {
            printDhtReadings();
        }

        double y = ReadTemperatureTermistor();

        if (x == 0)
        {
            ox = MAP_X(x, plot, screen);
            oy = MAP_Y(y, plot, screen);
        }
        else
        {
            Graph(
                tft, // &d name of your display object
                screen,
                plot,
                x,      // x = x data point
                y,      // y = y datapont
                GREEN); // pcolor = color of your plotted data
            delay(250);
        }
    }
}

#endif // TERMISTOR_OUTPUT_H