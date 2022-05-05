
#include <spo2_algorithm.h>
#include "Spo2MAX30102Adapter.h"

//static uint8_t spo2Symbol[] = {
// PROGMEM legt das Bild auf den Flash statt in den RAM
//static const PROGMEM uint8_t spo2Symbol[] = {
//static const PROGMEM unsigned char spo2Symbol[] = {
static unsigned char spo2Symbol[] = {
    B11100000, B00000000, B00000000, B00000111,
    B10000000, B00000000, B00000000, B00000001,
    B10000000, B00000000, B00000000, B00000001,
    B00000000, B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000, B00000000,
    B00000000, B00000000, B00000000, B00000000,
    B00000000, B00001111, B11110000, B00000000,
    B00000000, B11110000, B00001111, B00000000,
    B00000001, B11000000, B00000011, B10000000,
    B00000011, B10000000, B00000001, B11000000,
    B00000110, B00000000, B00000000, B01100000,
    B00000110, B00000000, B00000000, B01100000,
    B00001100, B00000000, B00000000, B00110000,
    B00001100, B00000000, B00000000, B00110000,
    B00001100, B00000000, B00000000, B00110000,
    B00000110, B00000000, B00000000, B01100000,
    B00000011, B10000000, B00000001, B11000000,
    B00000001, B11000000, B00000011, B10000000,
    B00000000, B11110000, B00001111, B00000000,
    B00000000, B00001111, B11110000, B00000000,
    B00000000, B00000000, B00000000, B00000000,
    B00000000, B00000000, B00001111, B00000000,
    B00000000, B00000000, B00010000, B10000000,
    B00000000, B00000000, B00010000, B01000000,
    B00000000, B00000000, B00000000, B10000000,
    B00000000, B00000000, B00000001, B00000000,
    B00000000, B00000000, B00000010, B00000000,
    B00000000, B00000000, B00000100, B00000000,
    B00000000, B00000000, B00001000, B00000000,
    B10000000, B00000000, B00011111, B11000001,
    B10000000, B00000000, B00000000, B00000001,
    B11100000, B00000000, B00000000, B00000111};

const unsigned int mLength = 9;
unsigned char messageActiveSpo2[mLength] = "SPO2: ";

// Konstruktor
Spo2MAX30102Adapter::Spo2MAX30102Adapter(Display *display, MAX30105 *sensor)
{
    this->sensor = sensor;
    this->display = display;
}

// Baue die Verbindung über I2C zum MAX30102 auf und konfiguriere ihn mit Setupwerten zur Spo2-Messung
void Spo2MAX30102Adapter::initialize()
{

    /* Possible settings are:
	byte ledBrightness = 60;    //Options: 0=Off to 255=50mA
	byte sampleAverage = 4;     //Options: 1, 2, 4, 8, 16, 32
	byte ledMode = 2;           //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
	byte sampleRate = 100;      //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
	int pulseWidth = 411;       //Options: 69, 118, 215, 411
	int adcRange = 4096;        //Options: 2048, 4096, 8192, 16384*/

    sensor->begin(Wire, I2C_SPEED_FAST); // Default I2C port, 400kHz speed
    //sensor->begin(Wire);      // so ist es bei BPM
    sensor->setup(60, 4, 2, 100, 411, 4096); //Sensor mit diesen Parametern konfigurieren
}

// Wecke den Sensor auf, führe erneut den Setup durch, setze BufferPosition zurück
void Spo2MAX30102Adapter::startUp()
{
    initialBufferPosition = 0;
    sensor->wakeUp();
    sensor->setup(60, 4, 2, 100, 411, 4096);
}

void Spo2MAX30102Adapter::run()
{
    long irValue = sensor->getIR();

    if (irValue > IR_THRESHOLD)
    {

        // Misst zunächst "BUFFER_LENGTH" (100) Spo2-Werte, um diese Mitteln zu können
        if (initialBufferPosition < BUFFER_LENGTH)
        {
            display->print(spo2Symbol, CALCULATING_STRING, 14);
            if (sensor->available())
            {
                redBuffer[initialBufferPosition] = sensor->getRed();
                irBuffer[initialBufferPosition] = sensor->getIR();
                initialBufferPosition++;
            }
        }
        else
        {
            // Berechne Spo2-Wert aus gemessenen Bufferdaten, Validiert den bestimmten Wert
            maxim_heart_rate_and_oxygen_saturation(irBuffer, BUFFER_LENGTH, redBuffer, &spo2, &validSpo2, &h, &vh);

            // Gibt den Spo2 Wert am Display aus, falls dieser valide ist
            if (validSpo2)
            {

                char avgSpo2Value[4];

                sprintf(avgSpo2Value, "%d", spo2);
                messageActiveSpo2[6] = avgSpo2Value[0];
                messageActiveSpo2[7] = avgSpo2Value[1];
                messageActiveSpo2[8] = avgSpo2Value[2];

                display->print(spo2Symbol, messageActiveSpo2, mLength);

                //Serial.println(spo2);
            }

            // Schreibt nächsten Wert in Buffer
            if (runningBufferPosition < BUFFER_LENGTH)
            {
                if (sensor->available())
                {
                    redBuffer[runningBufferPosition] = sensor->getRed();
                    irBuffer[runningBufferPosition] = sensor->getIR();
                    runningBufferPosition++;
                }
            }
            else
            {
                runningBufferPosition = 0;
            }
        }
    }
    else
    {
        // Falls kein "Finger" erkannt wurde, gebe am Display "No Finger!" aus
        display->print(spo2Symbol, MESSAGE_INACTIVE, 14);
    }
}

// Versetzt MAX30102 in den Ruhezustand
void Spo2MAX30102Adapter::end()
{
    sensor->shutDown();
}