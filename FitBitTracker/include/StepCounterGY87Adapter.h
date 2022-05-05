#pragma once

#include "Module.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define MOVEMENT_THRESHOLD 2.0f

class StepCounterGY87Adapter : public Module
{
public:
    void initialize();

    void startUp();

    void run();

    void end();

    void runBackground();

    StepCounterGY87Adapter(Display *display, Adafruit_MPU6050 *sensor);

private:
    Display *display;
    Adafruit_MPU6050 *sensor;

    bool highPoint = false;
    bool lowPoint = true;
    int stepCounter = 0;
};