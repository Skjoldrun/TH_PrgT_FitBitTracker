
#include "DisplayAdafruit128x32OledAdapter.h"

DisplayAdafruit128x32OledAdapter::DisplayAdafruit128x32OledAdapter(Adafruit_SSD1306 *display, int i2c_address)
{
	this->display = display;
	this->i2c_address = i2c_address;
}

void DisplayAdafruit128x32OledAdapter::initialize()
{
	if (!display->begin(SSD1306_SWITCHCAPVCC, i2c_address))
	{
		Serial.println("Display Allocation failed");
		for (;;)
			;
	}

	display->clearDisplay();
}

void DisplayAdafruit128x32OledAdapter::print(unsigned char *logo, unsigned char *data, int data_length)
{
	display->clearDisplay();

	// Print Data
	data_length = data_length > 14 ? 14 : data_length;
	display->setTextColor(SSD1306_WHITE);
	display->setTextSize(1);
	display->setCursor(40, 12);
	//display->setCursor(50, 0);
	display->cp437(true);
	for (int16_t i = 0; i < data_length; i++)
	{
		display->write(data[i]);
	}

	// Print Logo
	display->drawBitmap(
		0, 0,
		logo,
		logo_width, logo_width,
		SSD1306_WHITE);

	display->display();
}
