#ifndef CANVAS_H
#define CANVAS_H

#include "trace_type.h"
#include "box.h"

class Trace;
class Adafruit_TFTLCD;

class Canvas
{
private:
    const int ControlsPaneWidth = 0;

    Adafruit_TFTLCD *display;
    box plot_space;
    box disp_space;
    Trace *traces[TraceType::TraceTypeLength];

public:
    Canvas(Adafruit_TFTLCD *display, box plot_space);
    ~Canvas();

    Adafruit_TFTLCD *GetDisplay();
    void Draw(TraceType type, double x, double y);
};

#endif // CANVAS_H