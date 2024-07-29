#include "hardware.h"
#include "pages.h"
#include "Adafruit_DRV2605.h"

// #include "dsp.h"
void toggleLed() {
	static bool ledState = false;
	digitalWrite(LED_BUILTIN, ledState);
	ledState = !ledState;
	Serial.println("Led toggled");
}

void updateButtons() {
	btnEnc.loop();
}

void testVirbration() {
	haptic.go();
	Serial.println("Vibrating");
}

// TFT_eSPI tft = TFT_eSPI();
void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	while (!Serial)
	{
		/* code */
	}
	delay(1000); // delay here to wait for serial monitor to connect
	Serial.println("Hello, World!");
	// digitalWrite(LED_BUILTIN, HIGH);
	// Serial.println("LED on");
	// tft.init();
	// PageManager.init();
	// hardwareSetup();
	// // blink led
	// pinMode(LED_BUILTIN, OUTPUT);
	taskManager.scheduleFixedRate(1000, toggleLed);
	
	// taskManager.scheduleFixedRate(1, updateButtons);
	// taskManager.scheduleFixedRate(20, updatePage); // update page every 20ms (50Hz)
	// taskManager.scheduleFixedRate(1000, testVirbration);
}

void loop() {
	// put your main code here, to run repeatedly:
	taskManager.runLoop();
}