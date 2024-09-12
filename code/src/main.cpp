#include "hardware.h"
#include "pages.h"
#include "Adafruit_DRV2605.h"
#include "Prompter.h"

uint8_t adc_pins[] = {PIEZO_PIN};  //ADC1 common pins for ESP32S2/S3 + ESP32C3/C6 + ESP32H2
uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);
volatile bool adc_coversion_done = false;
volatile int adcValArr[256] = {0};
volatile int adcValIndex = 0;


void ARDUINO_ISR_ATTR adcComplete() {
	adc_continuous_data_t *result = NULL;
  	if (analogContinuousRead(&result, 0))
	{
		adcValArr[adcValIndex] = result[0].avg_read_raw;
		adcValIndex = adcValIndex + 1;
		if (adcValIndex >= 256)
		{
			adcValIndex = 0;
			adc_coversion_done = true;
		}
	}
	
}

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
	delay(5000); // delay here to wait for serial monitor to connect
	Serial.println("Hello, World!");

	hardwareSetup();
	// load all pages once to set user data
	PageManager.loadAll();
	// load the first page
	PageManager.loadFistPage(PAGE_TUNER);
	// blink led
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(PIEZO_PIN, OUTPUT);
	taskManager.scheduleFixedRate(1000, toggleLed);
	taskManager.scheduleFixedRate(1, updateButtons);
	taskManager.scheduleFixedRate(5, updateGui);
	taskManager.scheduleFixedRate(20, updatePage); // update page every 20ms (50Hz)
	taskManager.scheduleFixedRate(10, []() {
		if (adc_coversion_done)
		{
			adc_coversion_done = false;
			// find the max value
			int maxVal = 0;
			for (int i = 0; i < 256; i++)
			{
				if (adcValArr[i] > maxVal)
				{
					maxVal = adcValArr[i];
				}
			}
			Serial.println(maxVal);
		}
	});
	Serial.println("Setup done!");
	// taskManager.scheduleFixedRate(100, []() {
	// 	Serial.println(analogRead(PIEZO_PIN));
	// });

	// Optional for ESP32: Set the resolution to 9-12 bits (default is 12 bits)
  	analogContinuousSetWidth(12);

  	// Optional: Set different attenaution (default is ADC_11db)
  	analogContinuousSetAtten(ADC_0db);

  	// Setup ADC Continuous with following input:
  	// array of pins, count of the pins, how many conversions per pin in one cycle will happen, sampling frequency, callback function
  	analogContinuous(adc_pins, adc_pins_count, 1, 16000, &adcComplete);

  	// Start ADC Continuous conversions
  	analogContinuousStart();

}

void loop() {
	// put your main code here, to run repeatedly:
	taskManager.runLoop();
}