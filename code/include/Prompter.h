#ifndef PROMPTER_H
#define PROMPTER_H

#include <ArduinoJson.h> 
#include <string>
#include <vector>

struct Chord {
    std::string name;
    int startBeat;
    int endBeat;
};

struct MusicScore {
    std::string name;
    int BPM;
    int beatsPerMeasure;
    std::vector<Chord> chords;
};

class Prompter {
public:
    Prompter();
    void start();
    void stop();
    void setCurrentBar(int bar);
    int getCurrentBar();
    void setMusicScore(const MusicScore& score);
    void loadMusicScoreFromJSON(const String& json);
    void setSpeed(float speed);
    std::string getCurrentChord();
    std::string getNextChord();
    std::string getNextNextChord();
    int findChordIndexByBeat(int beat);
    void updateCurrentBeat();

private:
    MusicScore musicScore;
    int currentBeat;
    int currentBar;
    float speedMultiplier;
    bool isRunning;
    int displayTaskID;

    static void chordUpdateCallback(); 
};
extern Prompter prompter;

#endif // PROMPTER_H
