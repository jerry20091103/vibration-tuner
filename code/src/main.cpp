#include "hardware.h"
#include "pages.h"
#include "Adafruit_DRV2605.h"
#include "butterworth.h"
#include "esp_timer.h"
#include "Tuner.h"
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

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	// while (!Serial)
	// {
	// 	/* code */
	// }
	delay(1000); // delay here to wait for serial monitor to connect
	Serial.println("Hello, World!");

	hardwareSetup();
	// vector<float> data = { 2418.67,2414.33,2410.33,2399,2387.67,2380.67,2377.33,2374.67,2371,2362.33,2354.67,2351,2347.67,2343.33,2337.33,2330.67,2328,2328,2324.67,2316,2310.67,2306.67,2303,2293,2289.33,2282.33,2281.67,2282.67,2283.67,2281.33,2282.67,2282.67,2283.33,2282.67,2285.33,2282.67,2289,2291.33,2291.67,2295,2295,2292.33,2291,2287.33,2288.33,2289.33,2295.33,2298,2305.67,2311,2317.67,2328,2332.67,2337.33,2339.33,2344,2349.33,2346,2343,2336,2333,2328,2325.33,2322.33, 2322,2328.67,2332.33,2334.33,2334.33,2338,2342.33,2345,2346.67,2349.33,2352.33,2354.67,2357.67,2365.33,2391,2401.67,2410.33,2424.33,2433,2444,2452,2466.33,2477,2484.33,2490.33,2497,2473.33,2465.67,2454.33,2450.67,2442.33,2445,2438,2435.67,2433.67,2433.33,2431,2435.33,2442,2453.67,2455.33,2462.67,2464,2470.33,2477,2480.67,2477.33,2481,2481.67,2490,2490.67,2487.67,2485,2488,2486,2487.67,2486.33,2484.33,2490,2492.67,2487.67,2488.33,2480.33,2478, 2474,2473.33,2490,2504.67,2518,2551.67,2572.67,2591.67,2605.67,2611.33,2626.33,2637,2642.33,2645,2646,2651,2650.33,2646,2619.67,2605.67,2587,2568,2552.67,2531,2516,2499,2486.67,2465,2451,2435,2423,2412,2398.67,2385.33,2371,2361.33,2350.33,2337.33,2331.67,2326.33,2320,2313.33,2308.67,2300,2291.33,2284.67,2275.33,2265.33,2263,2255.33,2250.33,2243.33,2239.33,2234.67,2234.67,2230.33,2229.67,2226.67,2225.33,2217.33,2187.33,2176,2164,2156.67, 2144.67,2132,2122.33,2113,2108.67,2101.67,2097.67,2093.33,2089,2088.67,2094.33,2096.33,2100.67,2101.33,2108.67,2108,2114,2112,2110.67,2105.33,2108,2105.33,2103.67,2101.33,2105,2103.67,2107.67,2126.67,2153.67,2167,2184,2201.33,2217.33,2231.67,2243.33,2253,2267.67,2275,2287.33,2296.33,2306.33,2318,2327,2334.67,2343.33,2349.67,2362,2376.33,2383.33,2393,2403.33,2416.33,2425,2433.33,2441,2446,2454.33,2463.67,2473.33,2481,2492,2506,2516.33,2531};
	// Tuner tuner;
	uint64_t start = esp_timer_get_time();
	// tuner.testing(data);
	uint64_t end = esp_timer_get_time();
	Serial.println("Time taken: " + String(end - start) + "us");
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

	// play 440 hz with buzzer
	// tone(BUZZER_PIN, 440);
	// taskManager.scheduleFixedRate(100, []() {
	// 	Serial.println(analogRead(PIEZO_PIN));
	// });
}

void loop() {
	// put your main code here, to run repeatedly:
	taskManager.runLoop();
}