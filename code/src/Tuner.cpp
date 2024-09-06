#include <Tuner.h>
#include <butterworth.h>
#include <cmath>
#include <hardware.h>
#include <driver/adc.h>



Tuner tuner;

void Tuner::loadExpectedFrequencies() {
    expectedFrequencies[0] = 329.63; // -5
    expectedFrequencies[1] = 246.94; // -10
    expectedFrequencies[2] = 196.00; // -14 
    expectedFrequencies[3] = 146.83; // -19
    expectedFrequencies[4] = 110.00; // -24
    expectedFrequencies[5] = 82.41; // -29
}

void Tuner::calculateExpectedFrequencies(float frequency) {
    expectedFrequencies[0] = frequency * pow(2, (-29 / 12.0));
    expectedFrequencies[1] = frequency * pow(2, (-24 / 12.0));
    expectedFrequencies[2] = frequency * pow(2, (-19 / 12.0));
    expectedFrequencies[3] = frequency * pow(2, (-14 / 12.0));
    expectedFrequencies[4] = frequency * pow(2, (-10 / 12.0));
    expectedFrequencies[5] = frequency * pow(2, (-5 / 12.0));
}

Tuner::Tuner()
{
    referenceFrequency = 440;
    loadExpectedFrequencies();
    reconstructFilter();
    uint8_t adc_pins[] = {PIEZO_PIN};
    uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);
    // adc_continuos_data_t* result = NULL;
    analogContinuousSetWidth(12);
    analogSetAttenuation(ADC_0db);

}

void Tuner::setReferenceFrequency(float frequency)
{
    referenceFrequency = frequency;
    calculateExpectedFrequencies(frequency);
    reconstructFilter();
}

void Tuner::reconstructFilter() {
    for (int i = 0; i < 6; i++) {
        int filterOrder = 3;
        double upperbound = expectedFrequencies[i] + 8 - float(i) / 2;
        double lowerbound = expectedFrequencies[i] - 8 - float(i) / 2;
        denCoeffs[i] = ComputeDenCoeffs(filterOrder, lowerbound, upperbound);
        numCoeffs[i] = ComputeNumCoeffs(filterOrder, lowerbound, upperbound, denCoeffs[i]);
    }
}

void Tuner::start() {

}