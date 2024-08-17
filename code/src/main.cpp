#include "hardware.h"
#include "pages.h"
#include "Adafruit_DRV2605.h"
#include "Prompter.h"

int buzzer_max = 0;
int buzzer_min = 4095;

void toggleLed() {
	static bool ledState = false;
	digitalWrite(LED_BUILTIN, ledState);
	ledState = !ledState;
}

void updateButtons() {
	btnEnc.loop();
}

// void testVirbration() {
// 	haptic.go();
// 	Serial.println("Vibrating");
// }

void check_buzzer_max() {
	Serial.println("Buzzer max: " + String(buzzer_max) + " min: " + String(buzzer_min));
	buzzer_max = 0;
	buzzer_min = 4095;
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
	// taskManager.scheduleFixedRate(1000, testVirbration);
	// taskManager.scheduleFixedRate(500, check_buzzer_max);

	// play 440 hz with buzzer
	// tone(BUZZER_PIN, 440);
	// set buzzer pin as input
	pinMode(BUZZER_PIN, INPUT);
	analogSetPinAttenuation(BUZZER_PIN, ADC_0db);

	String json = R"({
		"beatsPerMeasure": 4,
		"musicScore": [
			{
				"id": 0,
				"chords": [
					{"name": "C", "startBeat": 0, "endBeat": 1},
					{"name": "Am", "startBeat": 1, "endBeat": 2},
					{"name": "F", "startBeat": 2, "endBeat": 3},
					{"name": "G", "startBeat": 3, "endBeat": 4}
				]
			},
			{
				"id": 1,
				"chords": [
					{"name": "Em", "startBeat": 0, "endBeat": 1},
					{"name": "G", "startBeat": 1, "endBeat": 2},
					{"name": "C", "startBeat": 2, "endBeat": 3},
					{"name": "Am", "startBeat": 3, "endBeat": 4}
				]
			},
			{
				"id": 2,
				"chords": [
					{"name": "Dm", "startBeat": 0, "endBeat": 1},
					{"name": "G", "startBeat": 1, "endBeat": 2},
					{"name": "C", "startBeat": 2, "endBeat": 3},
					{"name": "E", "startBeat": 3, "endBeat": 4}
				]
			},
			{
				"id": 3,
				"chords": [
					{"name": "F", "startBeat": 0, "endBeat": 1},
					{"name": "Am", "startBeat": 1, "endBeat": 2},
					{"name": "Dm", "startBeat": 2, "endBeat": 3},
					{"name": "G", "startBeat": 3, "endBeat": 4}
				]
			},
			{
				"id": 4,
				"chords": [
					{"name": "C", "startBeat": 0, "endBeat": 1},
					{"name": "E", "startBeat": 1, "endBeat": 2},
					{"name": "Am", "startBeat": 2, "endBeat": 3},
					{"name": "F", "startBeat": 3, "endBeat": 4}
				]
			},
			{
				"id": 5,
				"chords": [
					{"name": "G", "startBeat": 0, "endBeat": 1},
					{"name": "C", "startBeat": 1, "endBeat": 2},
					{"name": "Em", "startBeat": 2, "endBeat": 3},
					{"name": "Am", "startBeat": 3, "endBeat": 4}
				]
			}
		]
	})";


	prompter.start();
	prompter.loadMusicScoreFromJSON(json);
	prompter.setCurrentBar(0); 

	prompter.getCurrentChord();
	prompter.getNextChord();
	prompter.getNextNextChord();



}

void loop() {
	// put your main code here, to run repeatedly:
	taskManager.runLoop();
	// read buzzer pin
	int buzzer_val = analogRead(BUZZER_PIN);
	if (buzzer_val > buzzer_max)
	{
		buzzer_max = buzzer_val;
	}
	if (buzzer_val < buzzer_min)
	{
		buzzer_min = buzzer_val;
	}
}