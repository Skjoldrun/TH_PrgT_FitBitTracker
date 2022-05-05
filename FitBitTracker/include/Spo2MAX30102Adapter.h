#pragma once

#include <MAX30105.h>
#include "Module.h"

#define BUFFER_LENGTH 100
#define CALCULATING_STRING "Calculating..."
#define MESSAGE_INACTIVE "No Finger!"
#define IR_THRESHOLD 7000

class Spo2MAX30102Adapter : public Module
{
public:
    // Initialisiere Modul in der Setup Methode
    void initialize();

    // Weckt den Sensor auf, wenn dieses Modul über die Knöpfe ausgewählt wird
    void startUp();

    // Wird kontinuierlich in der loop-Methode aufgerufen
    // Liest Sensor Daten aus und verarbeitet diese
    void run();

    // Versetzt Sensor in den Ruhezustand, wenn ein anderes Modul ausgewählt wird
    void end();

    //Konstruktor
    Spo2MAX30102Adapter(Display *display, MAX30105 *sensor);

private:
    // Timer um Herzrate akkurat darstellen zu können (Falls Wert von Anfang an angezeigt wird, ist dieser noch unpräzise)
    long startUpTimer = 0;

    // Pointer auf das Sensorobjekt (Wird im Konstruktor übergeben)
    MAX30105 *sensor;

    // Pointer auf das Displayobjekt (Wird im Konstruktor übergeben)
    Display *display;

    uint8_t initialBufferPosition = 0;
    uint8_t runningBufferPosition = 0;

    // für Nano/Uno
    uint16_t redBuffer[BUFFER_LENGTH];
    uint16_t irBuffer[BUFFER_LENGTH];
    // für MEGA2560
    //uint32_t redBuffer[BUFFER_LENGTH];
    //uint32_t irBuffer[BUFFER_LENGTH];

    int32_t spo2;
    int8_t validSpo2;
    int32_t h;
    int8_t vh;
};