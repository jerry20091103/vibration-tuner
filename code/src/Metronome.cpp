#include "Metronome.h"
#include "hardware.h"

Metronome metronome;

Metronome::Metronome()
{
    BPM = 120;
    isVibrating = false;
    vibrationTaskID = -1;
    beatCount = 4;
    currentBeat = 0;
    buzzerLevel = 100;
}

void Metronome::toggleVibrating()
{
    if (isVibrating)
    {
        stopVibrating();
    }
    else
    {
        startVibrating();
    }
}

bool Metronome::startVibrating(int currentBeat)
{
    this->currentBeat = currentBeat;
    if (!isVibrating)
    {
        isVibrating = true;
        int period = 60000000 / BPM;
        if (currentBeat == 0)
        {
            haptic.setWaveform(0, strongHapticWaveform);
            taskManager.scheduleOnce(metronome.STRONG_HAPTIC_DURATION, []() {
            analogWrite(BUZZER_PIN, 0);
            });
        }
        else
        {
            haptic.setWaveform(0, weakHapticWaveform);
            taskManager.scheduleOnce(metronome.WEAK_HAPTIC_DURATION, []() {
            analogWrite(BUZZER_PIN, 0);
            });
        }
        haptic.go();
        analogWrite(BUZZER_PIN, metronome.buzzerLevel);
        
        this->currentBeat++;
        vibrationTaskID = taskManager.scheduleFixedRate(period, vibrationCallback, TIME_MICROS);
        return true;
    }
    Serial.println("Error: Already vibrating");
    return false;
}

bool Metronome::stopVibrating()
{
    if (isVibrating)
    {
        isVibrating = false;
        taskManager.cancelTask(vibrationTaskID);
        return true;
    }
    Serial.println("Error: Not vibrating");
    return false;
}

void Metronome::setBPM(int BPM)
{
    if (BPM <= MAX_BPM && BPM >= MIN_BPM)
    {
        this->BPM = BPM;
    }
    if (BPM < MIN_BPM)
    {
        this->BPM = MIN_BPM;
    }
    if (BPM > MAX_BPM)
    {
        this->BPM = MAX_BPM;
    }
    if (isVibrating)
    {
        taskManager.cancelTask(vibrationTaskID);
        int period = 60000000 / BPM;
        vibrationTaskID = taskManager.scheduleFixedRate(period, vibrationCallback, TIME_MICROS);
    }
    Serial.println("Current BPM:" + String(BPM));
}

void Metronome::setBeatCount(int beatCount)
{
    this->beatCount = beatCount;
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
        taskManager.cancelTask(vibrationTaskID);
        int period = 60000000 / BPM;
        vibrationTaskID = taskManager.scheduleFixedRate(period, vibrationCallback, TIME_MICROS);
        Serial.println("Current BPM:" + String(BPM));
        return true;
    }
    Serial.println("Current BPM:" + String(BPM));
    return false;
}


void Metronome::vibrationCallback()
{
    metronome.currentBeat %= metronome.beatCount;
    if (!metronome.currentBeat)
    {
        haptic.setWaveform(0, metronome.strongHapticWaveform);
    }
    else
    {
        haptic.setWaveform(0, metronome.weakHapticWaveform);
    }
    haptic.go();
    analogWrite(BUZZER_PIN, metronome.buzzerLevel);
    if (!metronome.currentBeat)
    {
        taskManager.scheduleOnce(metronome.STRONG_HAPTIC_DURATION, []() {
            analogWrite(BUZZER_PIN, 0);
            });
    }
    else
    {
    taskManager.scheduleOnce(metronome.WEAK_HAPTIC_DURATION, []() {
        analogWrite(BUZZER_PIN, 0);
        });
    }
    metronome.currentBeat++;
    // Serial.println("Vibrating");
}

int Metronome::getBeatCount()
{
    return beatCount;
}

int Metronome::getCurrentBeat()
{
    return max(0, currentBeat - 1);
}

void Metronome::setStrongHapticWaveform(int waveform)
{
    strongHapticWaveform = waveform;
}

void Metronome::setWeakHapticWaveform(int waveform)
{
    weakHapticWaveform = waveform;
}

void Metronome::setBuzzerLevel(int level)
{
    buzzerLevel = level;
}