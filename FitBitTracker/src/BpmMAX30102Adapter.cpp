
#include "BpmMAX30102Adapter.h"

//static const PROGMEM unsigned char heart[] = {
//static const PROGMEM uint8_t heart[] = {
//static uint8_t heart[] = {
static unsigned char heart[] = {
	B11100000, B00000000, B00000000, B00000111,
	B10000000, B00000000, B00000000, B00000001,
	B10000000, B00000000, B00000000, B00000001,
	B00000000, B00000000, B00000000, B00000000,
	B00000000, B00000000, B00000000, B00000000,
	B00000000, B00000000, B00000000, B00000000,
	B00000000, B11100000, B00000111, B00000000,
	B00000001, B11110000, B00001111, B10000000,
	B00000011, B11111000, B00011111, B11000000,
	B00000111, B11111100, B00111111, B11100000,
	B00001111, B11111110, B01111111, B11110000,
	B00011111, B11111111, B11111111, B11111000,
	B00011111, B11111111, B11111111, B11111000,
	B00011111, B11111111, B11111111, B11111000,
	B00001111, B11111111, B11111111, B11110000,
	B00000111, B11111111, B11111111, B11100000,
	B00000011, B11111111, B11111111, B11000000,
	B00000001, B11111111, B11111111, B10000000,
	B00000000, B11111111, B11111111, B00000000,
	B00000000, B01111111, B11111110, B00000000,
	B00000000, B00111111, B11111100, B00000000,
	B00000000, B00011111, B11111000, B00000000,
	B00000000, B00001111, B11110000, B00000000,
	B00000000, B00000111, B11100000, B00000000,
	B00000000, B00000011, B11000000, B00000000,
	B00000000, B00000001, B10000000, B00000000,
	B00000000, B00000001, B10000000, B00000000,
	B00000000, B00000000, B00000000, B00000000,
	B00000000, B00000000, B00000000, B00000000,
	B10000000, B00000000, B00000000, B00000001,
	B10000000, B00000000, B00000000, B00000001,
	B11100000, B00000000, B00000000, B00000111};

unsigned char messageActive[9] = "BPM: ";

BpmMAX30102Adapter::BpmMAX30102Adapter(Display *display, MAX30105 *sensor)
{
	this->display = display;
	this->sensor = sensor;
}

// Baue die Verbindung über I2C zum MAX30102 auf und konfiguriere ihn mit Standardsetup
void BpmMAX30102Adapter::initialize()
{
	sensor->begin(Wire);
	sensor->setup();
	sensor->setPulseAmplitudeRed(0x0A);
	heartRate = 0;
}

// Weckt den MAX30102 auf und führt den Standardsetup durch
void BpmMAX30102Adapter::startUp()
{
	startUpTimer = 0;
	sensor->wakeUp();
	sensor->setup();
}

void BpmMAX30102Adapter::run()
{
	// Misst derzeitigen IR Wert
	long irValue = sensor->getIR();
	char avgBpmValue[4];

	// Gibt gemessenen BPM Wert (berechnet in der vorherigen Iteration) am Display aus
	sprintf(avgBpmValue, "%d", beatAvg);
	messageActive[5] = avgBpmValue[0];
	messageActive[6] = avgBpmValue[1];
	messageActive[7] = avgBpmValue[2];

	if (irValue > IR_THRESHOLD)
	{
		display->print(heart, messageActive, 9);

		// Prüfe auf Herzschlag, bestimme Zeit zum letzten Schlag und rechne Delta auf BPM um
		if (checkForBeat(irValue))
		{
			long delta = millis() - lastBeat;
			lastBeat = millis();

			heartRate = 60 / (delta / 1000.0);

			// Bestimme Mittelwert der letzten gemessenen Herzraten
			if (20 < heartRate < 255)
			{
				rates[rateSpot++] = (byte)heartRate;
				rateSpot %= RATE_SIZE;

				beatAvg = 0;
				for (byte x = 0; x < RATE_SIZE; x++)
				{
					beatAvg += rates[x];
				}
				beatAvg /= RATE_SIZE;
			}
		}
	}
	else
	{
		// Falls kein "Finger" erkannt wurde, gebe am Display "No Finger!" aus
		display->print(heart, MESSAGE_INACTIVE, 14);
	}
}

// Versetze MAX30102 in den Ruhezustand
void BpmMAX30102Adapter::end()
{
	sensor->shutDown();
}