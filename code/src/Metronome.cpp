#include "Metronome.h"
#include "hardware.h"

Metronome metronome;

Metronome::Metronome()
{
    BPM = 120;
    isVibrating = false;
    vibrationTaskID = -1;
}

void Metronome::toggleVibrating()
{
    if (isVibrating)
    {
        isVibrating = false;
        taskManager.cancelTask(vibrationTaskID);
    }
    else
    {
        isVibrating = true;
        int period = 60000000 / BPM;
        haptic.go();
        Serial.println("Vibrating");
        vibrationTaskID = taskManager.scheduleFixedRate(period, vibrationCallback, TIME_MICROS);
    }
}

bool Metronome::getVibrating()
{
    return isVibrating;
}

int Metronome::getBPM()
{
    return BPM;
}

bool Metronome::updateBPM(int value)
{
    if (BPM + value <= MAX_BPM && BPM + value >= MIN_BPM)
    {
        BPM += value;
        Serial.println("Current BPM:" + String(BPM));
        return true;
    }
    Serial.println("Current BPM:" + String(BPM));
    return false;
}

void Metronome::vibrationCallback()
{
    haptic.go();
    Serial.println("Vibrating");
}