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
The tuner use the piezo input to detect the frequency of the sound and display it on the screen. This is single note detection for now.
### Piezo input
- Currently connected to the analog input pin D9.
- The `analogSetPinAttenuation()` is set to 0dB for the piezo input.
- Use `analogRead()` to read the input value.
- It may be better to use `analogReadContinuous()` to get the input value at a set interval (not tested yet).
- visit https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/adc.html for more info on the ADC and continuous reading.
### Functions
- start(): 
    - Starts the tuner.
    - The tuner will start listening to the input and detect the frequency.
- stop():
    - Stops the tuner to save cpu in other modes.
- getFrequency():
    - Returns the frequency of the input sound.
    - Ideally, we can get a new value at least every 20ms (50Hz), because the screen refresh rate is 50Hz.
- getNote():
    - Returns the note number of the input sound.
    - If the frequency is not exactly a note, it will return the nearest note.
    - The note is in the format of [midi note number](https://inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies).
- getCent():
    - Returns the cent difference from the nearest note.
    - one cent is 1/100 of a semitone.
    - range is -50 to 50.
- setReferenceFrequency(int frequency):
    - Sets the reference frequency for the tuner in Hz
    - Default: 440H. The means the note A4 is defined as 440Hz.
    - range: 400-480Hz