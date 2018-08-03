#ifndef TERMISTOR_OUTPUT_H
#define TERMISTOR_OUTPUT_H

#include "dht_output.h"
#include "graph.h"
#include "temperature.h"

void drawTermistorReading()
{
    boolean display7 = true;

    for (double x = 0; x <= 60; x += 1)
    {
        double t = ReadTemperatureTermistor();

        if ((int)x % 5 == 0)
        {
            printDhtReadings();
        }

        Graph(
            tft,              // &d name of your display object
            x,                // x = x data point
            t,                // y = y datapont
            0,                // gx = x graph location (upper left) - needed for y axes numbers
            0,                // gy = y graph location (upper left) - needed for x axes numbers and title
            tft.width() - 1,  // w = width of graph                 - needed for y and last x axes numbers
            tft.height() - 1, // h = height of graph                - needed for title
            0,                // xlo = lower bound of x axis
            60,               // xhi = upper bound of x asis
            10,               // xinc = division of x axis (distance not count)
            20,               // ylo = lower bound of y axis
            30,               // yhi = upper bound of y asis
            1,                // yinc = division of y axis (distance not count)
            "Temparature",    // title = title of graph
            "sec",            // xlabel = x asis label
            "C",              // ylabel = y asis label
            DKBLUE,           // gcolor = graph line colors
            RED,              // acolor = axes
            GREEN,            // pcolor = color of your plotted data
            WHITE,            // tcolor = text color
            BLACK,            // bcolor = background color
            display7);        // &redraw = flag to redraw graph on fist call only

        delay(250);
    }
}

#endif // TERMISTOR_OUTPUT_H