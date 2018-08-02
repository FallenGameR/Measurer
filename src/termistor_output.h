#ifndef TERMISTOR_OUTPUT_H
#define TERMISTOR_OUTPUT_H

#include "dht_output.h"
#include "graph.h"

// Device objects

#define X_AXES_PADDING 50
#define Y_AXES_PADDING 30
#define TITLE_PADDING 30

// the value of the 'other' resistor
#define SERIESRESISTOR 1e4

// Termistor resistance at 25c
#define THERMISTORNOMINAL 1e4

// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25

// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950

void drawTermistorReading()
{
    boolean display7 = true;

    for (double x = 0; x <= 60; x += 1)
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

        double y = r;
        Serial.println(y);

        if ((int)x % 5 == 0)
        {
            printDhtReadings();
        }

        Graph(
            tft,                                           // &d name of your display object
            x,                                             // x = x data point
            steinhart,                                     // y = y datapont
            X_AXES_PADDING,                                // gx = x graph location (lower left) - needed for y axes numbers
            tft.height() - Y_AXES_PADDING,                 // gy = y graph location (lower left) - needed for x axes numbers and title
            tft.width() - X_AXES_PADDING - 1,              // w = width of graph                 - needed for y and last x axes numbers
            tft.height() - Y_AXES_PADDING - TITLE_PADDING, // h = height of graph                - needed for title
            0,                                             // xlo = lower bound of x axis
            60,                                            // xhi = upper bound of x asis
            10,                                            // xinc = division of x axis (distance not count)
            20,                                            // ylo = lower bound of y axis
            30,                                            // yhi = upper bound of y asis
            1,                                             // yinc = division of y axis (distance not count)
            "Temparature",                                 // title = title of graph
            "seconds",                                     // xlabel = x asis label
            "celcius",                                     // ylabel = y asis label
            DKBLUE,                                        // gcolor = graph line colors
            RED,                                           // acolor = axi ine colors
            GREEN,                                         // pcolor = color of your plotted data
            WHITE,                                         // tcolor = text color
            BLACK,                                         // bcolor = background color
            display7);                                     // &redraw = flag to redraw graph on fist call only

        delay(250);
    }

    tft.fillScreen(BLACK);
}

#endif // TERMISTOR_OUTPUT_H