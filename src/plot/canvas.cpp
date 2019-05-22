#include "canvas.h"
#include "trace.h"

Canvas::Canvas(Adafruit_TFTLCD *display, box plot_space)
{
    this->display = display;
    this->plot_space = plot_space;
    this->disp_space = {0, display->width() - 1.0 - ControlsPaneWidth, 0, display->height() - 1.0 - ControlsPaneWidth};
}

Canvas::~Canvas()
{
    for (auto &&trace : traces)
    {
        delete (trace);
    }
}

Adafruit_TFTLCD *Canvas::GetDisplay()
{
    return this->display;
}

void Canvas::Draw(TraceType type, double x, double y)
{
    x = MAP_X(x, this->plot_space, this->disp_space);
    y = MAP_Y(y, this->plot_space, this->disp_space);

    if (traces[type])
    {
        traces[type]->DrawTo(x, y);
    }
    else
    {
        traces[type] = new Trace(this, type, x, y);
    }
}