#ifndef TUNER_H
#define TUNER_H
#include <vector>
#include "Hardware.h"


class Tuner
{
private:
    const int filterOrder = 3;
    const int windowSize = 256;
    const int hopSize = 256;
    const float threshold = 0.2;
    const float sampleRate = 16000;
    float referenceFrequency;
    float expectedFrequencies[6];
    std::vector<std::vector<double> > numCoeffs;
    std::vector<std::vector<double> > denCoeffs;
    float calculatePitch(std::vector<float>& data2pitch);
    float get_pitch(std::vector<float>& cmndf, float threshold);
    std::vector<float> difference_function(std::vector<float>& data2pitch, int windowSize);
    std::vector<float> cumulative_mean_normalized_difference_function(std::vector<float>& diff);

    // adc continuous
    uint8_t adc_pins[1] = {PIEZO_PIN};
    uint8_t adc_pins_count = sizeof(adc_pins) / sizeof(uint8_t);
public:
    Tuner();
    void start();
    void stop();
    std::vector<float> getFrequncy();
    std::vector<float> getExpectedFrequencies();
    void setReferenceFrequency(float frequency);
    void reconstructFilter();
    void calculateExpectedFrequencies(float frequency);
    float testing(std::vector<double> data, int stringId);
    // pitch cal for taskmanager
    static void calculate6StringsPitch();
    // set which string to calculate pitch
    static void setStringToCalculate(int stringId);
}; 

extern Tuner tuner;

#endif
