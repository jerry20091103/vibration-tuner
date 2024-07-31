#ifndef METRONOME_H
#define METRONOME_H

#include "hardware.h"

class Metronome 
{
public:
    Metronome();
    void toggleVibrating();
    bool getVibrating();
    int getBPM();
    bool updateBPM(int value);
private:
    const int MAX_BPM = 600;
    const int MIN_BPM = 1;
    int BPM;
    bool isVibrating;
    int vibrationTaskID;
    static void vibrationCallback();
};
extern Metronome metronome;

#endif // METRONOME_H