#include "hardware.h"
#include "pages.h"

void toggleLed() {
	static bool ledState = false;
	digitalWrite(LED_BUILTIN, ledState);
	ledState = !ledState;
}

void updateButtons() {
	btnEnc.loop();
	btn0.loop();
}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	hardwareSetup();
	PageManager.init();
	// blink led
	pinMode(LED_BUILTIN, OUTPUT);
	taskManager.scheduleFixedRate(1000, toggleLed);
	
	taskManager.scheduleFixedRate(2, updateButtons);
	taskManager.scheduleFixedRate(20, updatePage); // update page every 20ms (50Hz)
}

void loop() {
	// put your main code here, to run repeatedly:
	taskManager.runLoop();
}