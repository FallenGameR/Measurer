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

#define LEGEND_PADDING_Y_HORIZONTAL -20
#define LEGEND_PADDING_Y_VERTICAL -5
#define LEGEND_PADDING_X_HORIZONTAL -12
#define LEGEND_PADDING_X_VERTICAL 5
#define TITLE_PADDING_HORIZONTAL LEGEND_PADDING_Y_HORIZONTAL
#define TITLE_PADDING_VERTICAL -30

/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

*/
void Graph(
    Adafruit_ILI9341 &d, // display object
    double x,            // x data point
    double y,            // y data point
    double gx,           // x graph location (upper left corner)
    double gy,           // y graph location (upper left corner)
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
    double i;
    double temp;

    if (redraw == true)
    {
        // Don't redraw axes on the next run
        redraw = false;

        // Initialize old x and old y in order to draw the first point of the graph
        // This transform funcition is the same as the map function, except the map uses long and we use doubles
        ox = (x - xlo) * (w) / (xhi - xlo) + gx;
        oy = (y - ylo) * (-h) / (yhi - ylo) + gy + h;

        // Draw y scale
        for (i = ylo; i <= yhi; i += yinc)
        {
            temp = (i - ylo) * (-h) / (yhi - ylo) + gy + h;
            d.drawLine(gx, temp, gx + w, temp, (i == 0) ? acolor : gcolor);
            d.setTextSize(1);
            d.setTextColor(tcolor, bcolor);
            d.setCursor(gx + LEGEND_PADDING_Y_HORIZONTAL, temp + LEGEND_PADDING_Y_VERTICAL);
            d.println((int)i);
        }

        // Draw x scale
        for (i = xlo; i <= xhi; i += xinc)
        {
            temp = (i - xlo) * (w) / (xhi - xlo) + gx;
            d.drawLine(temp, gy + h, temp, gy, (i == 0) ? acolor : gcolor);
            d.setTextSize(1);
            d.setTextColor(tcolor, bcolor);
            d.setCursor(temp + LEGEND_PADDING_X_HORIZONTAL, gy + h + LEGEND_PADDING_X_VERTICAL);
            d.println((int)i);
        }

        // Draw legend
        d.setTextSize(2);
        d.setTextColor(tcolor, bcolor);
        d.setCursor(gx + TITLE_PADDING_HORIZONTAL, gy + TITLE_PADDING_VERTICAL);
        d.print(title);

        d.setTextSize(1);
        d.setTextColor(acolor, bcolor);
        d.setCursor(gx, gy + h + 20);
        d.print(xlabel);

        d.setTextSize(1);
        d.setTextColor(acolor, bcolor);
        d.setCursor(gx - 30, gy - 10);
        d.print(ylabel);
    }

    // Plot the data in as a bold line
    x = (x - xlo) * (w) / (xhi - xlo) + gx;
    y = (y - ylo) * (-h) / (yhi - ylo) + gy + h;
    d.drawLine(ox, oy, x, y, pcolor);
    d.drawLine(ox, oy + 1, x, y + 1, pcolor);
    d.drawLine(ox, oy - 1, x, y - 1, pcolor);
    ox = x;
    oy = y;
}

#endif // GRAPH_H