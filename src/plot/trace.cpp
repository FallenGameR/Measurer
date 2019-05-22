#include "trace.h"
#include "canvas.h"
#include "colors.h"
#include <Adafruit_TFTLCD.h>

Trace::Trace(Canvas *canvas, TraceType type, double x0, double y0)
{
    this->canvas = canvas;
    this->type = type;
    this->lastSegment = {x0, 0, y0, 0};
}

unsigned int Trace::GetColor()
{
    // TODO: map from type to color
    return GREEN;
}

void Trace::DrawTo(double x, double y)
{
    // preparation
    unsigned int color = this->GetColor();
    double x0 = this->lastSegment.xlo;
    double y0 = this->lastSegment.ylo;
    this->lastSegment.xlo = x;
    this->lastSegment.ylo = y;

    // bold line 3 pixel in width
    this->canvas->GetDisplay()->drawLine(x0, y0 - 1, x, y - 1, color);
    this->canvas->GetDisplay()->drawLine(x0, y0 + 0, x, y + 0, color);
    this->canvas->GetDisplay()->drawLine(x0, y0 + 1, x, y + 1, color);
}