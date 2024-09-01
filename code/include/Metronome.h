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
    void setBPM(int BPM);
    bool startVibrating(int currentBeat = 0);
    bool stopVibrating();
    void setBeatCount(int beatCount);
    int getBeatCount();
    int getCurrentBeat();
    void setStrongHapticWaveform(int waveform);
    void setWeakHapticWaveform(int waveform);
    void setBuzzerLevel(int level);
private:
    const int MAX_BPM = 600;
    const int MIN_BPM = 1;
    const int STRONG_HAPTIC_DURATION = 150;
    const int WEAK_HAPTIC_DURATION = 30;
    int BPM;
    bool isVibrating;
    uint8_t vibrationTaskID;
    int beatCount;
    int currentBeat;
    int buzzerLevel;
    int strongHapticWaveform;
    int weakHapticWaveform;

    static void vibrationCallback();
};
extern Metronome metronome;

#endif // METRONOME_H