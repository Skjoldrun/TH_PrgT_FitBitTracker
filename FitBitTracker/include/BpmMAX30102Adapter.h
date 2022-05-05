#pragma once

#include <MAX30105.h>
#include "Module.h"
#include "Display.h"
#include "heartRate.h"

// Grenze des gemessenen Infrarot-Wertes (ca. 0 - 120000) ab dem erkannt wird, dass der Sensor angelegt ist
#define IR_THRESHOLD 7000
// Glättet gemessenen Herzschlag je Größer die RATE_SIZE
#define RATE_SIZE 4

#define MESSAGE_INACTIVE "No Finger!"

class BpmMAX30102Adapter : public Module
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

    // Konstruktor
    BpmMAX30102Adapter(Display *display, MAX30105 *sensor);

private:
    // Timer um Herzrate akkurat darstellen zu können (Falls Wert von Anfang an angezeigt wird, ist dieser noch unpräzise)
    long startUpTimer = 0;

    // Pointer auf das Sensorobjekt (Wird im Konstruktor übergeben)
    MAX30105 *sensor;

    // Pointer auf das Displayobjekt (Wird im Konstruktor übergeben)
    Display *display;

    // Zum Glätten von Ausreißern
    byte rates[RATE_SIZE];
    byte rateSpot = 0;

    // Zeitstempel des letzten erkannten Herzschlages
    long lastBeat = 0;

    // Durchschnittliche (geglättete) Herzschlagrate
    byte beatAvg;
    byte heartRate;
};
