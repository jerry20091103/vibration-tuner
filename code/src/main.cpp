#include "hardware.h"
#include "pages.h"
#include "Adafruit_DRV2605.h"
#include "butterworth.h"
#include "esp_timer.h"
#include "Tuner.h"
#include "Prompter.h"

#define SERIAL_SIZE_RX 1024

void toggleLed() {
	static bool ledState = false;
	digitalWrite(LED_BUILTIN, ledState);
	ledState = !ledState;
}

void updateButtons() {
	btnEnc.loop();
}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	// while (!Serial)
	// {
	// 	/* code */
	// }
	delay(1000); // delay here to wait for serial monitor to connect
	Serial.println("Hello, World!");
  Serial.setRxBufferSize(SERIAL_SIZE_RX);

	hardwareSetup();
	// load all pages once to set user data
	PageManager.loadAll();
	// load the first page
	PageManager.loadFistPage(PAGE_TUNER);

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(PIEZO_PIN, OUTPUT);
	taskManager.scheduleFixedRate(1000, toggleLed);
	taskManager.scheduleFixedRate(1, updateButtons);
	taskManager.scheduleFixedRate(5, updateGui);
	taskManager.scheduleFixedRate(20, updatePage); // update page every 20ms (50Hz)
	taskManager.scheduleFixedRate(1, Tuner::calculate6StringsPitch); // calculate pitch every 4ms (250Hz)
	Serial.println("Setup done!");
	// taskManager.scheduleFixedRate(100, []() {
	// 	Serial.println(analogRead(PIEZO_PIN));
	// });
}

void loop()
{
    // put your main code here, to run repeatedly:
    taskManager.runLoop();
}