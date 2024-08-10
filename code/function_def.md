# Function Description for vibration-tuner
- all datatypes/names are for reference only, can be changed as needed
## Metronome
The metronome allows the user to set a tempo and have the device vibrate at that tempo.
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

## Prompter
The prompter is a text display for the user to see the current chord to play. It need to sync with the metronome.
### Chords
- The chord can be represented as ascii string. About 5~7 characters should be enough.
- We plan to display the current, next, and next next chord. (3~4 chords)
- Here is a guitar chord finder website: https://www.oolimo.com/en/guitar-chords/find. There are some common chords used for reference.
- Here are some examples: "C", "G", "Am", "F", "Dm", "E7", "G7", "Cmaj7", "Dm7", "Fm7", "C/E", "G/B", "D/F#", "Cdim7" ...
### Timeline
- The chords are stored with time information. We use can use BPM as time unit to better sync with the metronome? (or Bar count can be used?)
- For now we provide 1 "slot" per beat to write chords. This means the minimum time duration is 1 beat per chord. The duration can be any multiple of 1 beat.
- For example, here is a random timeline with 4 beats per measure: ("|" denotes a metronome beat)
```
|C  |A   |    |F   |G   |
C starts at beat 0 (bar 0),     ends at beat 1 (bar 0.25)
A starts at beat 1 (bar 0.25),  ends at beat 3 (bar 0.75)
F starts at beat 3 (bar 0.75),  ends at beat 4 (bar 1)
G starts at beat 4 (bar 1),     ends at beat 5 (bar 1.25)
```
### Music Score
- The music score shoud contain the following info: (can add more if needed)
    - BPM: Beats per minute.
    - Beats per measure: The number of beats in a measure.
    - Chords: A list of chords to play and their time information.
    - name? 
- 
### Functions
- start():
    - Starts the prompter.
    - The prompter will start displaying the chords.
    - The metronome should also be started at the same time.
- stop():
    - Stops the prompter.
    - The prompter will stop displaying the chords.
    - The metronome should also be stopped at the same time.
    - The prompter should be able to resume from the last position. when called start() again.
- setCurrentBar(int bar):
    - Sets the current bar number.
    - one Bar means one measure.
    - The "beats per measure" info need to be provided by the music score.
    - This allows the user to scroll to a specific part of a song with encoder turn.
- getCurrentBar():
    - Returns the current bar number.
- setMusicScore(MusicScore score):
    - Sets the music score for the prompter.
    - The music score contains all the info needed for the prompter.
    - The prompter will display the chords according to the music score.
    - This may be a json string or a struct.
    - or maybe score loading shoud be done in the backend instead of frontend?
- setSpeed(float speed):
    - Sets the speed of the prompter.
    - The speed is a multiplier of the BPM.
    - For example, if the BPM is 120 and the speed is 0.5, the prompter and metronome will operate at 60 BPM.
    - The speed can be changed during the prompter is running.
    - range is 0.5-2.0.
- getCurrentChord():
    - Returns the current chord to play.
    - The prompter will display the current chord on the screen.
- getNextChord():
    - Returns the next chord to play.
    - The prompter will display the next chord on the screen.
- getNextNextChord():
    - Returns the next next chord to play.
    - The prompter will display the next next chord on the screen.

  