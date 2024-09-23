#include <Arduino.h>
#include <Tuner.h>
#include <cmath>
#include <hardware.h>
#include "driver/adc.h"
#include <realtime_filter.h>
#include <butterworth.h>
#include <float.h>

using namespace std;
Tuner tuner;

#define WINDOW_SIZE 256

// void Tuner::loadExpectedFrequencies() {
//     expectedFrequencies[5] = 329.63; // -5
//     expectedFrequencies[4] = 246.94; // -10
//     expectedFrequencies[3] = 196.00; // -14 
//     expectedFrequencies[2] = 146.83; // -19
//     expectedFrequencies[1] = 110.00; // -24
//     expectedFrequencies[0] = 82.41; // -29
// }

// volatile variable for ISR
volatile bool adc_coversion_done = false;
volatile int adcValArr[WINDOW_SIZE] = {0};
volatile int adcValIndex = 0;

// avg pitch for 6 strings
float avgPitch[6] = {0};
int stringCounter = 0;

void ARDUINO_ISR_ATTR adcComplete() {
	adc_continuous_data_t *result = NULL;
  	if (analogContinuousRead(&result, 0))
	{
		adcValArr[adcValIndex] = result[0].avg_read_raw;
		adcValIndex = adcValIndex + 1;
		if (adcValIndex >= WINDOW_SIZE)
		{
			adcValIndex = 0;
			adc_coversion_done = true;
		}
	}
}

void Tuner::calculateExpectedFrequencies(float frequency) {
    expectedFrequencies[5] = frequency * pow(2, (-5 / 12.0));
    expectedFrequencies[4] = frequency * pow(2, (-10 / 12.0));
    expectedFrequencies[3] = frequency * pow(2, (-14 / 12.0));
    expectedFrequencies[2] = frequency * pow(2, (-19 / 12.0));
    expectedFrequencies[1] = frequency * pow(2, (-24 / 12.0));
    expectedFrequencies[0] = frequency * pow(2, (-29 / 12.0));
}

void Tuner::calculate6StringsPitch()
{
    if (adc_coversion_done)
	{
		adc_coversion_done = false;
		// calculate pitch
		uint64_t start = esp_timer_get_time();
        float pitch = 0;	
        pitch = tuner.testing(std::vector<double>(adcValArr, adcValArr + WINDOW_SIZE), stringCounter);
		uint64_t end = esp_timer_get_time();
		// running average for 30 samples
		avgPitch[stringCounter] = (avgPitch[stringCounter] * 29 + pitch) / 30;
	}
}

void Tuner::setStringToCalculate(int stringId)
{
    stringCounter = stringId;
}

Tuner::Tuner()
{
    referenceFrequency = 440;
    numCoeffs.resize(6);
    denCoeffs.resize(6);
    for (int i = 0; i < 6; i++) {
        numCoeffs[i].resize(7);
        denCoeffs[i].resize(7);
    }
    calculateExpectedFrequencies(referenceFrequency);
    reconstructFilter();
    // setup adc
    analogContinuousSetWidth(12);
    analogContinuousSetAtten(ADC_0db);
    analogContinuous(adc_pins, adc_pins_count, 1, sampleRate, &adcComplete);
    // analogRead(PIEZO_PIN);
}

void Tuner::setReferenceFrequency(float frequency)
{
    referenceFrequency = frequency;
    calculateExpectedFrequencies(frequency);
    reconstructFilter();
}

void Tuner::reconstructFilter() {
    int filterOrder = 3;
    for (int i = 0; i < 6; i++) {
        double upperbound = expectedFrequencies[i] + 8 - float(i) / 2;
        double lowerbound = expectedFrequencies[i] - 8 - float(i) / 2;
        // Serial.println("lowerbound:");
        denCoeffs[i] = ComputeDenCoeffs(filterOrder, lowerbound / (sampleRate / 2), upperbound / (sampleRate / 2));
        numCoeffs[i] = ComputeNumCoeffs(filterOrder, lowerbound / (sampleRate / 2), upperbound / (sampleRate / 2), denCoeffs[i]);
        // Serial.println("lowerbound:");
        // Serial.println(lowerbound, 10);
        // Serial.println("upperbound:");
        // Serial.println(upperbound, 10);
        // Serial.println("denCoeffs:");
        // for (auto &j: denCoeffs[i]) {
        //     Serial.println(j, 10);
        // }
        // Serial.println("numCoeffs:");
        // for (auto &j: numCoeffs[i]) {
        //     Serial.println(j, 10);
        // }
    }
}

void Tuner::start() {
    analogContinuousStart();
}

void Tuner::stop() {
    analogContinuousStop();
    adc_coversion_done = false;
}

std::vector<float> Tuner::getFrequncy() {
    std::vector<float> freqs(6);
    for (int i = 0; i < 6; i++) {
        freqs[i] = avgPitch[i];
    }
    return freqs;
}

std::vector<float> Tuner::getExpectedFrequencies() {
    std::vector<float> freqs(6);
    for (int i = 0; i < 6; i++) {
        freqs[i] = expectedFrequencies[i];
    }
    return freqs;
}

float Tuner::calculatePitch(vector<float>& data2pitch) {
    vector<float> diff = difference_function(data2pitch, windowSize); // 4004us
    vector<float> cmndf = cumulative_mean_normalized_difference_function(diff); // 5464us
    float tau = get_pitch(cmndf, threshold); // 5469 us
    if (tau > 0) {
        return sampleRate / tau;
    }
    else {
        return 0;
    }
    // return 0;
}

vector<float> Tuner::difference_function(vector<float>& signal, int w_size) {
    vector<float> diff(w_size);
    for (int tau = 0; tau < w_size; tau++) {
        float sum = 0;
        for (int j = 0; j < w_size - tau; j++) {
            sum += (signal[j] - signal[j + tau]) * (signal[j] - signal[j + tau]);
            //TODO: change to esp-dsp optimized version
        }
        diff[tau] = sum;
    }
    return diff;
}

float Tuner::get_pitch(vector<float>& cmndf, float threshold) {
    float pitch = 0;
    for (int tau = 1; tau < cmndf.size(); tau++) {
        if (cmndf[tau] < threshold) {
            while (tau + 1 < cmndf.size() && cmndf[tau + 1] < cmndf[tau]) {
                tau++;
            }
            pitch = tau;
            break;
        }
    }
    return pitch;
}


vector<float> Tuner::cumulative_mean_normalized_difference_function(vector<float>& diff) {
    int w_size = diff.size();
    vector<float> cmndf(w_size);
    cmndf[0] = 1;
    for (int tau = 1; tau < w_size; tau++) {
        float sum = 0;
        for (int j = 1; j < tau; j++) {
            sum += diff[j];
        }
        cmndf[tau] = diff[tau] / sum;
    }
    return cmndf;
}

float Tuner::testing(vector<double> data, int stringId) {
    vector<double> filteredData(data.size()); // 84us
    // Serial.println("numCoeefs:");
    // for (auto &i: numCoeffs[0]) {
    //     Serial.println(i);
    // }
    // Serial.println("denCoeffs");
    // for (auto &i: denCoeffs[0]) {
    //     Serial.println(i);
    // }

    // run filter for all 6 strings
    // for (int i = 0; i < 1; i++) {
    //     if (stringId != i)
    //         realtime_filter(data, numCoeffs[i], denCoeffs[i], i);
    //     else // only get filtered data for the current string
    //         filteredData = realtime_filter(data, numCoeffs[i], denCoeffs[i], i);
    // }
    filteredData = realtime_filter(data, numCoeffs[stringId], denCoeffs[stringId], stringId);
    // for (auto &i: filteredData) {
    //     Serial.println(i);
    // }
    vector<float> floatFilteredData(filteredData.begin(), filteredData.end());
    float pitch = calculatePitch(floatFilteredData); // 5480us
    return pitch;
    // return 0;
}

//256

// sr 16k


// 15987us


// sr 16k

// 256

// 64

// 64/16000


// 250hz

// 4 ms 4000us