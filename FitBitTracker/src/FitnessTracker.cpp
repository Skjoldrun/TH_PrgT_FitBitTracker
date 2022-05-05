
#include <Arduino.h>
#include "FitnessTracker.h"

// Konstruktor
FitnessTracker::FitnessTracker(Display *dis, Module **modules, uint8_t moduleCount)
{
	this->display = dis;
	this->modules = modules;
	this->currentView = modules[0];
	this->moduleCount = moduleCount;
}

void FitnessTracker::initialize()
{
	// Initialisiert Display
	display->initialize();
	Serial.println("Display initialisiert.");

	// Initialisiert jedes Modul
	for (int i = 0; i < moduleCount; i++)
	{
		modules[i]->initialize();
		Serial.print("Modul: ");
		Serial.print(i);
		Serial.println(" initialisiert.");
	}

	// Startet erstes Modul
	currentView->startUp();
	Serial.println("Modul gestartet");
}

void FitnessTracker::run()
{
	// Führe Berechnungen des derzeit ausgewählten Moduls durch
	currentView->run();

	for (int i = 0; i < moduleCount; i++)
	{
		if (modules[i]->runBackgroundEnabled)
		{
			modules[i]->runBackground();
		}
	}

	// Falls "switchModule"-Flag gesetzt wurde (falls Button gedrückt wurde)
	if (switchModule)
	{
		// Fahre derzeitiges Modul herunter
		currentView->end();

		// Wähle und aktiviere nächstes Modul
		currentView = modules[currentViewIndex];
		currentView->startUp();

		switchModule = false;

		// Print out in Serial Monitor
		Serial.println("Modul wurde gewechselt.");
	}
}

// Entprellt Eingabe durch den Button, bestimmt Index des nächsten Moduls, setzt "switchModule"-Flag
void FitnessTracker::OnButtonPressed(bool forward)
{

	long interruptTime = millis();

	if (interruptTime - millisLastInterrupt > BUFFER_TIME)
	{
		// Bestimme Index des nächsten Elements in der Liste der Module
		if (forward)
		{
			currentViewIndex++;
			if (currentViewIndex >= moduleCount)
			{
				currentViewIndex = 0;
			}
		}
		else
		{
			currentViewIndex--;
			if (currentViewIndex < 0)
			{
				currentViewIndex = moduleCount - 1;
			}
		}

		switchModule = true;
	}

	millisLastInterrupt = interruptTime;
	Serial.print("Button aktiviert: ");
	Serial.println(switchModule);
}