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

        Graph(
            tft, // &d name of your display object
            x,   // x = x data point
            t,   // y = y datapont
            screen,
            plot,
            10,            // xinc = division of x axis (distance not count)
            1,             // yinc = division of y axis (distance not count)
            "Temparature", // title = title of graph
            "sec",         // xlabel = x asis label
            "C",           // ylabel = y asis label
            DKBLUE,        // gcolor = graph line colors
            RED,           // acolor = axes
            GREEN,         // pcolor = color of your plotted data
            WHITE,         // tcolor = text color
            BLACK,         // bcolor = background color
            display7);     // &redraw = flag to redraw graph on fist call only

        delay(250);
    }
}

#endif // TERMISTOR_OUTPUT_H