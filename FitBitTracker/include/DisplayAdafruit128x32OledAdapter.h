#pragma once

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Display.h"

class DisplayAdafruit128x32OledAdapter : public Display
{
public:
    // Initialisiere Display (Baue I2C Verbindung auf)
    void initialize() override;

    // Drucke Daten:
    // 1. Links ein Symbol (32x32) das das ausgewählte Modul beschreibt (Bspw. Herz bei BPM-Modul)
    // 2. Rechts die von den Modulen ermittelten Daten
    void print(unsigned char *logo, unsigned char *data, int data_length) override;

    DisplayAdafruit128x32OledAdapter(Adafruit_SSD1306 *display, int i2c_adress);

private:
    int i2c_address = 0x3C;
    int16_t logo_width = 32;
    Adafruit_SSD1306 *display;
};
