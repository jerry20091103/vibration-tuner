#include <Arduino.h>
#include <Tuner.h>
#include <cmath>
#include <hardware.h>
#include "driver/adc.h"
#include <realtime_filter.h>
#include <butterworth.h>
#include <float.h>

using namespace std;
// Tuner tuner;

// void Tuner::loadExpectedFrequencies() {
//     expectedFrequencies[5] = 329.63; // -5
//     expectedFrequencies[4] = 246.94; // -10
//     expectedFrequencies[3] = 196.00; // -14 
//     expectedFrequencies[2] = 146.83; // -19
//     expectedFrequencies[1] = 110.00; // -24
//     expectedFrequencies[0] = 82.41; // -29
// }

void Tuner::calculateExpectedFrequencies(float frequency) {
    expectedFrequencies[5] = frequency * pow(2, (-5 / 12.0));
    expectedFrequencies[4] = frequency * pow(2, (-10 / 12.0));
    expectedFrequencies[3] = frequency * pow(2, (-14 / 12.0));
    expectedFrequencies[2] = frequency * pow(2, (-19 / 12.0));
    expectedFrequencies[1] = frequency * pow(2, (-24 / 12.0));
    expectedFrequencies[0] = frequency * pow(2, (-29 / 12.0));
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
    uint8_t adc_pins[] = {PIEZO_PIN};
    uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);
    // // adc_continuos_data_t* result = NULL;
    // // analogSetAttenuation(ADC_0db);
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
    vector<double> rawData(hopSize);
    vector<double> filteredData(hopSize);
    filteredData = realtime_filter(rawData, numCoeffs[0], denCoeffs[0]);
    vector<float> data2pitch(windowSize);
    float pitch =calculatePitch(data2pitch);
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

float Tuner::testing(vector<double> data) {
    vector<double> filteredData(data.size()); // 84us
    // Serial.println("numCoeefs:");
    // for (auto &i: numCoeffs[0]) {
    //     Serial.println(i);
    // }
    // Serial.println("denCoeffs");
    // for (auto &i: denCoeffs[0]) {
    //     Serial.println(i);
    // }
    filteredData = realtime_filter(data, numCoeffs[0], denCoeffs[0]); // 2649us
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