#ifndef TUNER_H
#define TUNER_H
#include <vector>

using namespace std;

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
    vector<vector<double> > numCoeffs;
    vector<vector<double> > denCoeffs;
    float calculatePitch(vector<float> data2pitch);
    vector<float> difference_function(vector<float> data2pitch, int windowSize);
    vector<float> cumulative_mean_normalized_difference_function(vector<float> diff);
    float get_pitch(vector<float> cmndf, float threshold);
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
    float testing(vector<float> data);
}; 
#endif
