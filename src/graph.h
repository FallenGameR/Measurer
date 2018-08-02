#ifndef GRAPH_H
#define GRAPH_H

#include <Adafruit_ILI9341.h>
#include "colors.h"
#include "pins.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_MOSI, PIN_TFT_CLK, PIN_TFT_RST, PIN_TFT_MISO);

// this is the only external variable used by the graph
// it's a flat to draw the coordinate system only on the first pass
boolean display7 = true;
double ox, oy;

/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

*/
void Graph(
    Adafruit_ILI9341 &d, // display object
    double x,            // x data point
    double y,            // y data point
    double gx,           // x graph location (lower left corner)
    double gy,           // y graph location (lower left corner)
    double w,            // width of graph
    double h,            // height of graph
    double xlo,          // lower bound of x axis
    double xhi,          // upper bound of x asis
    double xinc,         // increments on x axis
    double ylo,          // lower bound of y axis
    double yhi,          // upper bound of y asis
    double yinc,         // increments on y axis
    String title,        // title of graph
    String xlabel,       // x axis label
    String ylabel,       // y axis label
    unsigned int gcolor, // grid color
    unsigned int acolor, // axes color
    unsigned int pcolor, // plotted data color
    unsigned int tcolor, // text color
    unsigned int bcolor, // background color
    boolean &redraw)     // flag to redraw graph on fist call only, when true draws axes and sets the flag to false
{
    // initialize old x and old y in order to draw the first point of the graph
    // but save the transformed value
    // note my transform funcition is the same as the map function, except the map uses long and we need doubles
    //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    double i;
    double temp;

    if (redraw == true)
    {
        redraw = false;
        ox = (x - xlo) * (w) / (xhi - xlo) + gx;
        oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
        // draw y scale
        for (i = ylo; i <= yhi; i += yinc)
        {
            // compute the transform
            temp = (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

            if (i == 0)
            {
                d.drawLine(gx, temp, gx + w, temp, acolor);
            }
            else
            {
                d.drawLine(gx, temp, gx + w, temp, gcolor);
            }

            d.setTextSize(1);
            d.setTextColor(tcolor, bcolor);
            d.setCursor(gx - 40, temp);
            // precision is default Arduino--this could really use some format control
            d.println((int)i);
        }
        // draw x scale
        for (i = xlo; i <= xhi; i += xinc)
        {

            // compute the transform

            temp = (i - xlo) * (w) / (xhi - xlo) + gx;
            if (i == 0)
            {
                d.drawLine(temp, gy, temp, gy - h, acolor);
            }
            else
            {
                d.drawLine(temp, gy, temp, gy - h, gcolor);
            }

            d.setTextSize(1);
            d.setTextColor(tcolor, bcolor);
            d.setCursor(temp, gy + 10);
            // precision is default Arduino--this could really use some format control
            d.println((int)i);
        }

        //now draw the labels
        d.setTextSize(2);
        d.setTextColor(tcolor, bcolor);
        d.setCursor(gx, gy - h - 30);
        d.println(title);

        d.setTextSize(1);
        d.setTextColor(acolor, bcolor);
        d.setCursor(gx, gy + 20);
        d.println(xlabel);

        d.setTextSize(1);
        d.setTextColor(acolor, bcolor);
        d.setCursor(gx - 30, gy - h - 10);
        d.println(ylabel);
    }

    //graph drawn now plot the data
    // the entire plotting code are these few lines...
    // recall that ox and oy are initialized as static above
    x = (x - xlo) * (w) / (xhi - xlo) + gx;
    y = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    d.drawLine(ox, oy, x, y, pcolor);
    d.drawLine(ox, oy + 1, x, y + 1, pcolor);
    d.drawLine(ox, oy - 1, x, y - 1, pcolor);
    ox = x;
    oy = y;
}

#endif // GRAPH_H