#ifndef GRAPH_H
#define GRAPH_H

#include <Adafruit_TFTLCD.h>
#include <Adafruit_GFX.h>
#include "plot/box.h"
#include "plot/macro.h"
#include "colors.h"
#include "pins.h"

Adafruit_TFTLCD tft = Adafruit_TFTLCD(PIN_TFT_CS, PIN_TFT_CD, PIN_TFT_WR, PIN_TFT_RD, PIN_TFT_RST);

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

void TftSetup()
{
    Serial.print(F("TFT size is "));
    Serial.print(tft.width());
    Serial.print(F("x"));
    Serial.println(tft.height());

    tft.reset();
    uint16_t identifier = tft.readID();

    if (identifier == 0x9341)
    {
        Serial.println(F("Found expected ILI9341 screen"));
    }
    else
    {
        Serial.print(F("Unknown LCD driver chip: "));
        Serial.println(identifier, HEX);
        return;
    }

    tft.begin(identifier);
    tft.setRotation(3);
    tft.fillScreen(BLACK);
}

void InitializeGrid(
    Adafruit_TFTLCD &d, // display object
    box &screen,
    box &plot,
    double xinc,         // increments on x axis
    double yinc,         // increments on y axis
    unsigned int gcolor, // grid color
    unsigned int tcolor, // text color
    unsigned int bcolor  // background color
)
{
    // Axis y
    for (double y = plot.ylo + yinc; y <= plot.yhi; y += yinc)
    {
        double temp = MAP_Y(y, plot, screen);
        d.drawLine(screen.xlo, temp, screen.xhi, temp, gcolor);
        d.setTextSize(1);
        d.setTextColor(tcolor, bcolor);
        d.setCursor(screen.xlo + LEGEND_PADDING_Y_HORIZONTAL, temp + LEGEND_PADDING_Y_VERTICAL);
        d.print((int)y);
    }

    // Axis x
    for (double x = plot.xlo + xinc; x <= plot.xhi; x += xinc)
    {
        double temp = MAP_X(x, plot, screen);
        d.drawLine(temp, screen.ylo, temp, screen.yhi, gcolor);
        d.setTextSize(1);
        d.setTextColor(tcolor, bcolor);
        d.setCursor(temp + LEGEND_PADDING_X_HORIZONTAL, screen.yhi + LEGEND_PADDING_X_VERTICAL);
        d.print((int)x);
    }
}

void InitializeAxes(
    Adafruit_TFTLCD &d, // display object
    box &screen,
    box &plot,
    String title,        // title of graph
    String xlabel,       // x axis label
    String ylabel,       // y axis label
    unsigned int acolor, // axes color
    unsigned int tcolor, // text color
    unsigned int bcolor  // background color
)
{
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

void Graph(Adafruit_TFTLCD &d, box &screen, box &plot, box &line, unsigned int color)
{
    double x = MAP_X(line.xhi, plot, screen);
    double y = MAP_Y(line.yhi, plot, screen);
    d.drawLine(line.xlo, line.ylo - 1, x, y - 1, color);
    d.drawLine(line.xlo, line.ylo + 0, x, y + 0, color);
    d.drawLine(line.xlo, line.ylo + 1, x, y + 1, color);
    line.xlo = x;
    line.ylo = y;
}

unsigned long testFillScreen()
{
    unsigned long start = micros();
    tft.fillScreen(BLACK);
    tft.fillScreen(RED);
    tft.fillScreen(GREEN);
    tft.fillScreen(BLUE);
    tft.fillScreen(BLACK);
    return micros() - start;
}

unsigned long testText()
{
    tft.fillScreen(BLACK);
    unsigned long start = micros();
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(1);
    tft.println("Hello World!");
    tft.setTextColor(YELLOW);
    tft.setTextSize(2);
    tft.println(1234.56);
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.println(0xDEADBEEF, HEX);
    tft.println();
    tft.setTextColor(GREEN);
    tft.setTextSize(5);
    tft.println("Groop");
    tft.setTextSize(2);
    tft.println("I implore thee,");
    tft.setTextSize(1);
    tft.println("my foonting turlingdromes.");
    tft.println("And hooptiously drangle me");
    tft.println("with crinkly bindlewurdles,");
    tft.println("Or I will rend thee");
    tft.println("in the gobberwarts");
    tft.println("with my blurglecruncheon,");
    tft.println("see if I don't!");
    return micros() - start;
}

unsigned long testLines(uint16_t color)
{
    unsigned long start, t;
    int x1, y1, x2, y2,
        w = tft.width(),
        h = tft.height();

    tft.fillScreen(BLACK);

    x1 = y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        tft.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6)
        tft.drawLine(x1, y1, x2, y2, color);
    t = micros() - start; // fillScreen doesn't count against timing

    tft.fillScreen(BLACK);

    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        tft.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6)
        tft.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    tft.fillScreen(BLACK);

    x1 = 0;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        tft.drawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6)
        tft.drawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    tft.fillScreen(BLACK);

    x1 = w - 1;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6)
        tft.drawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6)
        tft.drawLine(x1, y1, x2, y2, color);

    return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2)
{
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height();

    tft.fillScreen(BLACK);
    start = micros();
    for (y = 0; y < h; y += 5)
        tft.drawFastHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5)
        tft.drawFastVLine(x, 0, h, color2);

    return micros() - start;
}

unsigned long testRects(uint16_t color)
{
    unsigned long start;
    int n, i, i2,
        cx = tft.width() / 2,
        cy = tft.height() / 2;

    tft.fillScreen(BLACK);
    n = min(tft.width(), tft.height());
    start = micros();
    for (i = 2; i < n; i += 6)
    {
        i2 = i / 2;
        tft.drawRect(cx - i2, cy - i2, i, i, color);
    }

    return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2)
{
    unsigned long start, t = 0;
    int n, i, i2,
        cx = tft.width() / 2 - 1,
        cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    n = min(tft.width(), tft.height());
    for (i = n; i > 0; i -= 6)
    {
        i2 = i / 2;
        start = micros();
        tft.fillRect(cx - i2, cy - i2, i, i, color1);
        t += micros() - start;
        // Outlines are not included in timing results
        tft.drawRect(cx - i2, cy - i2, i, i, color2);
    }

    return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color)
{
    unsigned long start;
    int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

    tft.fillScreen(BLACK);
    start = micros();
    for (x = radius; x < w; x += r2)
    {
        for (y = radius; y < h; y += r2)
        {
            tft.fillCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color)
{
    unsigned long start;
    int x, y, r2 = radius * 2,
              w = tft.width() + radius,
              h = tft.height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros();
    for (x = 0; x < w; x += r2)
    {
        for (y = 0; y < h; y += r2)
        {
            tft.drawCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testTriangles()
{
    unsigned long start;
    int n, i, cx = tft.width() / 2 - 1,
              cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    n = min(cx, cy);
    start = micros();
    for (i = 0; i < n; i += 5)
    {
        tft.drawTriangle(
            cx, cy - i,     // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            tft.color565(0, 0, i));
    }

    return micros() - start;
}

unsigned long testFilledTriangles()
{
    unsigned long start, t = 0;
    int i, cx = tft.width() / 2 - 1,
           cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    start = micros();
    for (i = min(cx, cy); i > 10; i -= 5)
    {
        start = micros();
        tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                         tft.color565(0, i, i));
        t += micros() - start;
        tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                         tft.color565(i, i, 0));
    }

    return t;
}

unsigned long testRoundRects()
{
    unsigned long start;
    int w, i, i2,
        cx = tft.width() / 2 - 1,
        cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    w = min(tft.width(), tft.height());
    start = micros();
    for (i = 0; i < w; i += 6)
    {
        i2 = i / 2;
        tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(i, 0, 0));
    }

    return micros() - start;
}

unsigned long testFilledRoundRects()
{
    unsigned long start;
    int i, i2,
        cx = tft.width() / 2 - 1,
        cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    start = micros();
    for (i = min(tft.width(), tft.height()); i > 20; i -= 6)
    {
        i2 = i / 2;
        tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(0, i, 0));
    }

    return micros() - start;
}

#endif // GRAPH_H