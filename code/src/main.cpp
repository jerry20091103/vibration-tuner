#include "hardware.h"
#include "pages.h"
#include "Adafruit_DRV2605.h"

void toggleLed() {
	static bool ledState = false;
	digitalWrite(LED_BUILTIN, ledState);
	ledState = !ledState;
}

void updateButtons() {
	btnEnc.loop();
}

void testVirbration() {
	haptic.go();
	Serial.println("Vibrating");
}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	while (!Serial)
	{
		/* code */
	}
	delay(1000); // delay here to wait for serial monitor to connect
	Serial.println("Hello, World!");
	hardwareSetup();
	// blink led
	pinMode(LED_BUILTIN, OUTPUT);
	taskManager.scheduleFixedRate(1000, toggleLed);
	
	taskManager.scheduleFixedRate(1, updateButtons);
	taskManager.scheduleFixedRate(5, updateGui);
	taskManager.scheduleFixedRate(20, updatePage); // update page every 20ms (50Hz)
	taskManager.scheduleFixedRate(1000, testVirbration);

	// play 440 hz with buzzer
	tone(BUZZER_PIN, 440);
}

void loop() {
	// put your main code here, to run repeatedly:
	taskManager.runLoop();
}