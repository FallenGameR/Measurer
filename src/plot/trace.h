#ifndef TRACE_H
#define TRACE_H

#include "trace_type.h"
#include "box.h"

class Canvas;

class Trace
{
private:
    Canvas *canvas;
    TraceType type;
    box lastSegment;

public:
    Trace(Canvas *canvas, TraceType type, double x0, double y0);
    unsigned int GetColor();
    box DrawTo(double x, double y);
};

#endif // TRACE_H