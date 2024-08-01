#ifndef METRONOME_H
#define METRONOME_H

#include "hardware.h"

class Metronome 
{
public:
    Metronome();
    void toggleVibrating();
    bool getVibrating();
    uint8_t getBPM();
    bool updateBPM(uint8_t value);
    void setBPM(uint8_t BPM);
    bool startVibrating();
    bool stopVibrating();
    void setBeatCount(uint8_t beatCount);
    uint8_t getBeatCount();
    uint8_t getCurrentBeat();
    void setStrongHapticWaveform(uint8_t waveform);
    void setWeakHapticWaveform(uint8_t waveform);
    void setBuzzerLevel(uint8_t level);
private:
    const uint8_t MAX_BPM = 600;
    const uint8_t MIN_BPM = 1;
    uint8_t BPM;
    bool isVibrating;
    uint8_t vibrationTaskID;
    uint8_t beatCount;
    uint8_t currentBeat;
    uint8_t buzzerLevel;
    uint8_t strongHapticWaveform;
    uint8_t weakHapticWaveform;
    
    static void vibrationCallback();
};
extern Metronome metronome;

#endif // METRONOME_H