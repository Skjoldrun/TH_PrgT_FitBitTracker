#pragma once

#include "Display.h"
#include "Module.h"
//#include "ClockModule.h"
//#include "BpmModule.h"

// Repräsentiert den Fitnesstracker (sozusagen, das Hauptobjekt einer Fitnesstracker Bibliothek)
// Steuert die verwendeten Module und verarbeitet die Eingabe über die Knöpfe

#define BUFFER_TIME 200

class FitnessTracker
{
public:
	// Im Konstruktor wird später eine Liste aus Modulen übergeben
	FitnessTracker(Display *display, Module **modules, uint8_t moduleCount);

	// Rufe alle intialize-Methoden der Module und des Displays auf
	void initialize();

	// TODO: In Run-Methode umbenennen
	void run();

	void OnButtonPressed(bool forward);

private:
	// TODO: Prüfen
	// Display wird evtl nicht direkt benötigt
	Display *display;

	// Liste aller Module
	Module **modules;
	int moduleCount = 0;

	// Pointer auf das derzeit aktive Modul
	Module *currentView;
	int currentViewIndex = 0;

	// Zum Buffern des Button Inputs
	long millisLastInterrupt = 0;
	bool switchModule = false;
};
