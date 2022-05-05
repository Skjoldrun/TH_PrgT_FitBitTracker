
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <MAX30105.h>
#include "DisplayAdafruit128x32OledAdapter.h"
#include "FitnessTracker.h"
#include "BpmMAX30102Adapter.h"
#include "Spo2MAX30102Adapter.h"
#include "StepCounterGY87Adapter.h"

// Anzahl Module
#define MODULE_COUNT (uint8_t)2

// Display defines
#define LCD_WIDTH 128 // OLED display width in pixel
#define LCD_HEIGTH 32 // OLED display height in pixel
#define OLED_RESET -1 // Reset pin # (-1: to share Arduino reset pin)

Adafruit_SSD1306 *ssd1306 = new Adafruit_SSD1306(LCD_WIDTH, LCD_HEIGTH, &Wire, OLED_RESET);
// Obacht: das "WIE" eine SSD1306 initialisiert wird, ist wichtig.
// es gibt unterschiedliche Initialisierungsmöglichkeiten, je nachdem welches Display verwendet wird
// Mehr Konstruktoren finden Sie hier: https://github.com/adafruit/Adafruit_SSD1306/blob/master/Adafruit_SSD1306.cpp#L217

// Es verpixelt:
// http://forum.espruino.com/conversations/269330/

// Objekte der Projektmodule
Display *display = new DisplayAdafruit128x32OledAdapter(ssd1306, 0x3C);
MAX30105 *max30102 = new MAX30105();
Adafruit_MPU6050 *mpu6050 = new Adafruit_MPU6050();

// Objekte der Adapter:
Module *bpm = new BpmMAX30102Adapter(display, max30102);
//Module *spo2 = new Spo2MAX30102Adapter(display, max30102);  // this is too big for the storage!
Module *stepCounter = new StepCounterGY87Adapter(display, mpu6050);

// Array mit eingebundenen Modulen
Module *modules[] = {bpm, stepCounter};

FitnessTracker tracker(display, modules, MODULE_COUNT);

// Interrupt
int interruptPin = 2;

// ISR bei Druck des Buttons (Entprellung in der Methode)
void forwardButtonPressed()
{
  tracker.OnButtonPressed(true);
}

void setup()
{
  // Serial Schnittstelle zum Debuggen
  Serial.begin(9600);

  // Konfiguriere Button für Interrupt Event
  pinMode(interruptPin, INPUT_PULLUP);
  digitalWrite(interruptPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(interruptPin), forwardButtonPressed, LOW);

  // Initialisiere FitnessTracker
  tracker.initialize();
}

void loop()
{
  tracker.run();
}
