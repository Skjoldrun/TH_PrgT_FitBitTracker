#pragma once

#include <Wire.h>
#include "Display.h"
#include "MAX30105.h"

// Interface das die nötigen Funktionen für ein Modul beschreibt.
// Modul: Kann über die Knöpfe ausgewählt werden und bildet dann auf dem Display das jeweilige Symbol und die gemessenen/berechneten/etc. Daten ab

class Module
{
public:
	// Wird in der Setup Methode aufgerufen
	virtual void initialize(){};

	// Wird aufgerufen, wenn dieses Modul ausgewählt wird (über Knöpfe)
	virtual void startUp(){};

	// Wird in der loop-Methode fortwährend aufgerufen
	virtual void run(){};

	// Wird aufgerufen, wenn anderes Modul gewählt wird (über Knöpfe)
	virtual void end(){};

	virtual void runBackground(){};

	bool runBackgroundEnabled = false;
};
