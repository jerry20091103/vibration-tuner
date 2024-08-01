# Function Description for vibration-tuner
- all datatypes/names are for reference only, can be changed as needed
## Metronomone
The metronomone allows the user to set a tempo and have the device vibrate at that tempo.
### Functions
- setBPM(int bpm): 
    - Sets the tempo of the device to the given BPM. Range: at least 30-300 BPM.
- start(): 
    - Starts the device vibrating at the set tempo.
    - The first beat will always be a strong beat. (currentBeat = 0)
- stop(): 
    - Stops the device from vibrating.
- setBeatCount(int beatCount): 
    - Sets the number of beats in a measure. Range: 1-16 beats.
    - Default: 4 beats.
    - For example, if the beat count is set to 3, the device will vibrate 3 times in a group to given BPM. The pattern will be "strong, weak, weak". Each group starts with a strong beat.
- getCurrentBeat(): 
    - Returns the current beat number in the measure.
    - Range: 0-(beatCount-1)
    - This is used to show the pattern on screen.
    - For example, if the "beat count" is set to 3, the "current beat" will be 0, 1, or 2. if the "current beat" is 2, the dispaly will show ". X ." where X is the current beat.
- setStrongHapticWaveform(int waveform): 
    - The haptic driver provides 128 different waveform options.
    - This function sets the waveform for the strong beat.
- setWeakHapticWaveform(int waveform): 
    - This function sets the waveform for the weak beat.
- setBuzzerLevel(int level): (if we have one...)
    - Sets the volume of the buzzer. Range: 0-10.
    - 0 is silent, 10 is the loudest.

## Tuner
