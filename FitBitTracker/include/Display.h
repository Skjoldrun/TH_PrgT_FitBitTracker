#pragma once

class Display
{
public:
    // Initialisiere das Display
    virtual void initialize(){};

    // Drucke Symbol/Logo und Daten
    virtual void print(unsigned char *logo, unsigned char *data, int data_length){};
};