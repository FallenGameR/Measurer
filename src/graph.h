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

#define TITLE_PADDING_HORIZONTAL 0
#define TITLE_PADDING_VERTICAL 2

#define AXES_NAME_PADDING_Y_HORIZONTAL 2
#define AXES_NAME_PADDING_Y_VERTICAL 11
#define AXES_NAME_PADDING_X_HORIZONTAL 0
#define AXES_NAME_PADDING_X_VERTICAL -21

#define LEGEND_PADDING_Y_HORIZONTAL 2
#define LEGEND_PADDING_Y_VERTICAL 2
#define LEGEND_PADDING_X_HORIZONTAL -12
#define LEGEND_PADDING_X_VERTICAL -8

#define MAP_X(x, xlo, xhi, gx, w) ((x - xlo) * w / (xhi - xlo) + gx)
#define MAP_Y(y, ylo, yhi, gy, h) ((y - ylo) * -h / (yhi - ylo) + gy + h)

#define MAP_XB(x, plot, screen) (screen.xlo + (x - plot.xlo) * (screen.xhi - screen.xlo) / (plot.xhi - plot.xlo))
#define MAP_YB(y, plot, screen) (screen.yhi - (y - plot.ylo) * (screen.yhi - screen.ylo) / (plot.yhi - plot.ylo))

struct box
{
    double xlo;
    double ylo;
    double xhi;
    double yhi;
};

void InitializeGraph(
    Adafruit_ILI9341 &d, // display object
    box &screen,
    box &plot,
    double xinc,         // increments on x axis
    double yinc,         // increments on y axis
    String title,        // title of graph
    String xlabel,       // x axis label
    String ylabel,       // y axis label
    unsigned int gcolor, // grid color
    unsigned int acolor, // axes color
    unsigned int tcolor, // text color
    unsigned int bcolor  // background color
)
{
    // Draw y scale
    for (double y = plot.ylo + yinc; y <= plot.yhi; y += yinc)
    {
        double temp = MAP_YB(y, plot, screen);
        d.drawLine(screen.xlo, temp, screen.xhi, temp, gcolor);
        d.setTextSize(1);
        d.setTextColor(tcolor, bcolor);
        d.setCursor(screen.xlo + LEGEND_PADDING_Y_HORIZONTAL, temp + LEGEND_PADDING_Y_VERTICAL);
        d.print((int)y);
    }

    // Draw x scale
    for (double x = plot.xlo + xinc; x <= plot.xhi; x += xinc)
    {
        double temp = MAP_XB(x, plot, screen);
        d.drawLine(temp, screen.ylo, temp, screen.yhi, gcolor);
        d.setTextSize(1);
        d.setTextColor(tcolor, bcolor);
        d.setCursor(temp + LEGEND_PADDING_X_HORIZONTAL, screen.yhi + LEGEND_PADDING_X_VERTICAL);
        d.print((int)x);
    }

    // Draw title
    d.setTextSize(2);
    d.setTextColor(tcolor, bcolor);
    d.setCursor(screen.xhi - title.length() * 6 * 2 + TITLE_PADDING_HORIZONTAL, screen.ylo + TITLE_PADDING_VERTICAL);
    d.print(title);

    // Draw y axes
    d.drawLine(screen.xlo, screen.ylo, screen.xlo, screen.yhi, acolor);
    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(screen.xlo + AXES_NAME_PADDING_Y_HORIZONTAL, screen.ylo + AXES_NAME_PADDING_Y_VERTICAL);
    d.print(ylabel);

    // Draw x axes
    d.drawLine(screen.xlo, screen.yhi, screen.xhi, screen.yhi, acolor);
    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(screen.xhi - xlabel.length() * 6 + AXES_NAME_PADDING_X_HORIZONTAL, screen.yhi + AXES_NAME_PADDING_X_VERTICAL);
    d.print(xlabel);
}

void InitializeFirstDot()
{
    // Initialize old x and old y in order to draw the first point of the graph
    // This transform funcition is the same as the map function, except the map uses long and we use doubles
    //ox = MAP_X(x, xlo, xhi, gx, w);
    //oy = MAP_Y(y, ylo, yhi, gy, h);
}

// Split into grid and graph
// Previous dots into arguments
/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

*/
void Graph(
    Adafruit_ILI9341 &d, // display object
    double x,            // x data point
    double y,            // y data point
    box &screen,
    box &plot,
    double xinc,         // increments on x axis
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
    if (redraw == true)
    {
        // Don't redraw axes on the next run
        redraw = false;

        // Initialize old x and old y in order to draw the first point of the graph
        // This transform funcition is the same as the map function, except the map uses long and we use doubles
        ox = MAP_XB(x, plot, screen);
        oy = MAP_YB(y, plot, screen);

        // Draw grid
        InitializeGraph(d, screen, plot, xinc, yinc, title, xlabel, ylabel, gcolor, acolor, tcolor, bcolor);
    }

    // Plot the data in as a bold line
    x = MAP_XB(x, plot, screen);
    y = MAP_YB(y, plot, screen);
    d.drawLine(ox, oy, x, y, pcolor);
    d.drawLine(ox, oy + 1, x, y + 1, pcolor);
    d.drawLine(ox, oy - 1, x, y - 1, pcolor);
    ox = x;
    oy = y;
}

#endif // GRAPH_H