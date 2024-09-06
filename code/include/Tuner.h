#ifndef TUNER_H
#define TUNER_H
#include <vector>

using namespace std;

class Tuner
{
private:
    float referenceFrequency;
    float expectedFrequencies[6];
    vector<vector<double> > numCoeffs;
    vector<vector<double> > denCoeffs;
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
}; 
#endif
