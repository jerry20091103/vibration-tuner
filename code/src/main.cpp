#include "hardware.h"
#include "pages.h"
#include "Adafruit_DRV2605.h"

#include "dsp.h"
void toggleLed() {
	static bool ledState = false;
	digitalWrite(LED_BUILTIN, ledState);
	ledState = !ledState;
	// Serial.println("Led toggled");
}

void updateButtons() {
	btnEnc.loop();
}

void testVirbration() {
	haptic.go();
	Serial.println("Vibrating");
}
void show_text();

TFT_eSPI tft = TFT_eSPI();
void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	while (!Serial)
	{
		/* code */
	}
	delay(1000); // delay here to wait for serial monitor to connect
	Serial.println("Hello, World!");
	PageManager.init();
	hardwareSetup();
	tft.init();
	// blink led
	pinMode(LED_BUILTIN, OUTPUT);
	taskManager.scheduleFixedRate(1000, toggleLed);
	taskManager.scheduleFixedRate(100, show_text);
	taskManager.scheduleFixedRate(1, updateButtons);
	taskManager.scheduleFixedRate(20, updatePage); // update page every 20ms (50Hz)
	// taskManager.scheduleFixedRate(1000, testVirbration);
}

void loop() {
	// put your main code here, to run repeatedly:
	taskManager.runLoop();
}

void show_text() {
	Serial.println("Showing text");
	tft.fillScreen(TFT_WHITE);
	tft.setCursor(0, 0);
	tft.setTextColor(TFT_WHITE);  tft.setTextSize(2);
	tft.println("LingShunLAB.com");
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	tft.println(1234.56);
	tft.setTextColor(TFT_RED);    tft.setTextSize(3);
	tft.println(0xDEADBEEF, HEX);
	tft.println();
	tft.setTextColor(TFT_GREEN);
	tft.setTextSize(5);
	tft.println("Groop");
	tft.setTextSize(2);
	tft.println("I implore thee,");
	//tft.setTextColor(TFT_GREEN,TFT_BLACK);
	tft.setTextSize(1);
	tft.println("my foonting turlingdromes.");
	tft.println("And hooptiously drangle me");
	tft.println("with crinkly bindlewurdles,");
	tft.println("Or I will rend thee");
	tft.println("in the gobberwarts");
	tft.println("with my blurglecruncheon,");
	tft.println("see if I don't!");
}