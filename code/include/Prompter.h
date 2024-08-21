#ifndef PROMPTER_H
#define PROMPTER_H

#include <ArduinoJson.h>

#include <string>
#include <vector>

struct Chord
{
    std::string name;
    int startBeat;
    int endBeat;
};

struct MusicScore
{
    int id;
    std::string title;
    int BPM;
    int beatsPerMeasure;
    std::vector<Chord> chords;
};

class Prompter
{
  public:
    Prompter();
    bool start();
    bool stop();
    double getCurrentBar();
    void setMusicScore(const MusicScore& score);
    void loadMusicScoreFromJSON(const String& json);
    void setSpeed(float speed);
    // std::string getCurrentChord();
    std::vector<std::string> getNext8Chord();
    void updateCurrentBeat();
    MusicScore getMusicScore(int id);
    void setCurrentBeat(int beat);
    int getCurrentBeat();
    std::string getChordAtBeat(int beat);
    std::string getCurrentChord();

  private:
    MusicScore musicScore;
    int currentBeat;
    int currentChord;
    float speedMultiplier;
    bool isRunning;
    int prompterTaskID;
    bool isScoreLoaded;

    static void chordUpdateCallback();
};

extern Prompter prompter;

#endif // PROMPTER_H