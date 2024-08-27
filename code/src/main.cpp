#include "hardware.h"
#include "pages.h"
#include "Adafruit_DRV2605.h"
#include "butterworth.h"
#include "esp_timer.h"
int buzzer_max = 0;
int buzzer_min = 4095;


double test_data[100] = {1,0.9511,0.809,0.5878,0.309,0,-0.309,-0.5878,-0.809,-0.9511,-1,-0.9511,-0.809,-0.5878,-0.309,-0,0.309,0.5878,0.809,0.9511,1,0.9511,0.809,0.5878,0.309,-0,-0.309,-0.5878,-0.809,-0.9511,-1,-0.9511,-0.809,-0.5878,-0.309,-0,0.309,0.5878,0.809,0.9511,1,0.9511,0.809,0.5878,0.309,-0,-0.309,-0.5878,-0.809,-0.9511,-1,-0.9511,-0.809,-0.5878,-0.309,0,0.309,0.5878,0.809,0.9511,1,0.9511,0.809,0.5878,0.309,-0,-0.309,-0.5878,-0.809,-0.9511,-1,-0.9511,-0.809,-0.5878,-0.309,0,0.309,0.5878,0.809,0.9511,1,0.9511,0.809,0.5878,0.309,-0,-0.309,-0.5878,-0.809,-0.9511,-1,-0.9511,-0.809,-0.5878,-0.309,0,0.309,0.5878,0.809,0.9511};
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


void butterworth_test() {
	int N = 100;
	double FrequencyBands[2] = { 0.05,0.95 };
	int FiltOrd = 3;
	vector<double> a;
	vector<double> b;
	vector<double> x(N);
	vector<double> y(N);
	a = ComputeDenCoeffs(FiltOrd, FrequencyBands[0], FrequencyBands[1]);
	for (int k = 0; k < a.size(); k++)
	{
		Serial.println("DenC is:");
		Serial.println(a[k], 6);
	}

	b = ComputeNumCoeffs(FiltOrd, FrequencyBands[0], FrequencyBands[1], a);
	for (int k = 0; k < b.size(); k++)
	{
		Serial.println("NumC is:");
		Serial.println(b[k], 6);
	}
	y = filter(x, b, a);
	for (int k = 0; k < y.size(); k++)
	{
		Serial.println(y[k], 6);
	}
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
	uint64_t start = esp_timer_get_time();
	butterworth_test();
	uint64_t end = esp_timer_get_time();
	Serial.println("Time taken: " + String(end - start) + "us");
	// blink led
	pinMode(LED_BUILTIN, OUTPUT);
	taskManager.scheduleFixedRate(1000, toggleLed);
	
	taskManager.scheduleFixedRate(1, updateButtons);
	taskManager.scheduleFixedRate(5, updateGui);
	taskManager.scheduleFixedRate(20, updatePage); // update page every 20ms (50Hz)

	// play 440 hz with buzzer
	tone(BUZZER_PIN, 440);
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