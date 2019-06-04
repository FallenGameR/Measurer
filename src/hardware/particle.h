#ifndef PARTICLE_H
#define PARTICLE_H

#include "ParticleSensor.h"
#include "..\graph.h"

ParticleSensor pmSensor;

void DrawPmSensor()
{
    ParticleReading *data = pmSensor.Read();
    // NULL case
    // auto pointers

    double x = 0;
    double y = 0;  //data->particles_03um; //
    double sd = 100;

    //delete data;

    // x interval:
    // 1 min = 60 sec
    // 10 min
    // 1 hour?
    // 1 day - bound to RTC

    // y interval - bound to data, ok to store some initial settings, refine them via the latest data

    // display many lines at the same time
    // do not waste RAM memory
    // ok to use flash alot
    // binary format for storage for random reads and writes, convert to csv

    box screen = {0, tft.width() - 1.0, 0, tft.height() - 1.0}; // const, but would need to leave space for controls
    box plot = {0, 60, y - 3 * sd, y + 3 * sd};
    box line = {MAP_X(x, plot, screen), 0, MAP_Y(y, plot, screen), 0};

    // Draw grid                          //
    InitializeGrid(tft, screen, plot, 10, 100, DKBLUE, WHITE, BLACK);
    //                                //                               //
    InitializeAxes(tft, screen, plot, "P > 0.3 um in 0.1L air", "sec", "particles", RED, WHITE, BLACK);

    // Draw graph
    for (double x = 1; x <= 60; x += 1)
    {
        data = pmSensor.Read();

        line.xhi = x;
        line.yhi = data->particles_03um; //
        Graph(tft, screen, plot, line, GREEN);
        delay(1000);

        delete data;

        //Serial.print("PM 1.0 (ug / m^3): ");
        //Serial.println(data.pm10_standard);
    }
}

#endif // PARTICLE_H