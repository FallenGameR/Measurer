#include "canvas.h"
#include "trace.h"
#include "macro.h"
#include <Adafruit_TFTLCD.h>

Canvas::Canvas(Adafruit_TFTLCD *display, box plot_space)
{
    this->display = display;
    this->plot_space = plot_space;
    this->disp_space.xlo = 0;
    this->disp_space.xhi = display->width() - 1.0 - Canvas::ControlsPaneWidth;
    this->disp_space.ylo = 0;
    this->disp_space.yhi = display->height() - 1.0 - Canvas::ControlsPaneWidth;
};

Canvas::~Canvas()
{
    for (auto &&trace : this->traces)
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