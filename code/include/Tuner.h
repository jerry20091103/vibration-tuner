#ifndef TUNER_H
#define TUNER_H
#include <vector>


class Tuner
{
private:
    const int filterOrder = 3;
    const int windowSize = 256;
    const int hopSize = 64;
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
public:
    Tuner();
    void start();
    void stop();
    float getFrequncy();
    int getNote();
    float getCent();
    void setReferenceFrequency(float frequency);
    void reconstructFilter();
    void loadExpectedFrequencies();
    void calculateExpectedFrequencies(float frequency);
    float testing(std::vector<double> data);
}; 
#endif
