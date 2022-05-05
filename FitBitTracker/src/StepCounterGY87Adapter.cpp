#include "StepCounterGY87Adapter.h"

//static const PROGMEM unsigned char boot[] = {
//static const PROGMEM uint8_t boot[] = {
static unsigned char boot[] = {
    B11100000, B00000000, B00000000, B00000111,
    B10000000, B00000000, B00000000, B00000001,
    B10000000, B00000000, B00000000, B00000001,
    B00000000, B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000, B00000000,
    B00000000, B00001111, B11111100, B00000000,
    B00000000, B00001111, B11111000, B00000000,
    B00000000, B00011111, B11111000, B00000000,
    B00000000, B00011111, B11110000, B00000000,
    B00000000, B00111111, B11110000, B00000000,
    B00000000, B00111111, B11100000, B00000000,
    B00000000, B01111111, B11100000, B00000000,
    B00000000, B01111111, B11000000, B00000000,
    B00000000, B11111111, B11000000, B00000000,
    B00000000, B11111111, B10000000, B00000000,
    B00000001, B11111111, B10000000, B00000000,
    B00000001, B11111111, B00000000, B00000000,
    B00000000, B11111111, B10000000, B00000000,
    B00000000, B11111111, B11000000, B00000000,
    B00000000, B01111111, B11100000, B00000000,
    B00000000, B00111111, B11110000, B00000000,
    B00000000, B00011111, B11111000, B00000000,
    B00000000, B00001111, B11111100, B00000000,
    B00011111, B11000111, B11111110, B00000000,
    B00000000, B00000011, B11111111, B00000000,
    B00011111, B11110001, B11111111, B10000000,
    B00000000, B00000000, B11111111, B00000000,
    B00011111, B11111100, B01111110, B00000000,
    B10000000, B00000000, B00111100, B00000001,
    B10000000, B00000000, B00000000, B00000001,
    B11100000, B00000000, B00000000, B00000111};

StepCounterGY87Adapter::StepCounterGY87Adapter(Display *display, Adafruit_MPU6050 *sensor)
{
    this->sensor = sensor;
    this->display = display;
    runBackgroundEnabled = true;
}

void StepCounterGY87Adapter::initialize()
{
    sensor->begin();
    sensor->setAccelerometerRange(MPU6050_RANGE_8_G);
    sensor->setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void StepCounterGY87Adapter::run()
{
    char steps[6];
    sprintf(steps, "%d", stepCounter);
    display->print(boot, steps, 6);
}

void StepCounterGY87Adapter::runBackground()
{
    sensors_event_t a, g, temp;

    sensor->getEvent(&a, &g, &temp);

    // Absolute Beschleunigung ermittelt aus 3 Achsen mit
    // Fallbeschleunigung - 9.81 m/s^2
    float currentAcceleration = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z) - 9.81f;

    // Gibt Daten auf Plotter/Serial Monitor aus.
    Serial.println(currentAcceleration);

    if ((currentAcceleration >= MOVEMENT_THRESHOLD) && !highPoint)
    {
        highPoint = true;
        lowPoint = false;
    }
    else if ((currentAcceleration <= 0) && !lowPoint)
    {
        stepCounter++;
        lowPoint = true;
        highPoint = false;
    }
}

void StepCounterGY87Adapter::startUp()
{
    // TODO: Test
}

void StepCounterGY87Adapter::end()
{
    // TODO: Test
}