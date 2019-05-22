#ifndef MACRO_H
#define MACRO_H

#define MAP_X(x, plot, screen) (screen.xlo + (x - plot.xlo) * (screen.xhi - screen.xlo) / (plot.xhi - plot.xlo))
#define MAP_Y(y, plot, screen) (screen.yhi - (y - plot.ylo) * (screen.yhi - screen.ylo) / (plot.yhi - plot.ylo))

#endif // MACRO_H