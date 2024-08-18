#include "Prompter.h"

#include "Metronome.h"

Prompter prompter;

Prompter::Prompter()
{
    currentBeat = 0;
    currentChord = 0;
    speedMultiplier = 1.0;
    isRunning = false;
    prompterTaskID = -1;
    isScoreLoaded = false;
}

bool Prompter::start()
{
    if (!isScoreLoaded)
    {
        Serial.println(
            "Error: Music score not loaded. Please load the music score before starting.");
        return false;
    }
    if (!isRunning)
    {
        Serial.println("Prompter started.");
        isRunning = true;
        int period = 60000000 / metronome.getBPM();
        prompterTaskID = taskManager.scheduleFixedRate(
            period, chordUpdateCallback, TIME_MICROS);
        return true;
    }
    Serial.println("Error: Prompter already start");
    return false;
}

bool Prompter::stop()
{
    if (isRunning)
    {
        Serial.println("Prompter stopped.");
        isRunning = false;
        taskManager.cancelTask(prompterTaskID);
        return true;
    }
    Serial.println("Error: Prompter already stop");
    return false;
}

void Prompter::setCurrentBeat(int beat)
{
    currentBeat = beat;
}

int Prompter::getCurrentBeat()
{
    return currentBeat;
}

double Prompter::getCurrentBar()
{
    return (double)currentBeat / musicScore.beatsPerMeasure;
}

void Prompter::setMusicScore(const MusicScore& score)
{
    musicScore = score;
    Serial.println("setMusicScore end");
    metronome.setBPM(score.BPM);
    isScoreLoaded = true;
}

void Prompter::loadMusicScoreFromJSON(const String& json)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error)
    {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }

    MusicScore score;

    score.BPM = doc["BPM"];
    score.beatsPerMeasure = doc["beatsPerMeasure"];

    JsonArray jsonChords = doc["chords"];
    for (JsonObject chordData : jsonChords)
    {
        Chord chord;
        chord.name = chordData["name"].as<std::string>();
        chord.startBeat = chordData["startBeat"];
        chord.endBeat = chordData["endBeat"];
        score.chords.push_back(chord);
    }

    setMusicScore(score);
}

void Prompter::setSpeed(float speed)
{
    if (speed >= 0.5 && speed <= 2.0)
    {
        speedMultiplier = speed;
        metronome.setBPM(metronome.getBPM() * speedMultiplier);
        Serial.print("Speed set to: ");
        Serial.println(speedMultiplier);
    }
}

std::string Prompter::getChordAtBeat(int beat)
{
    for (const Chord& chord : musicScore.chords)
    {
        if (beat >= chord.startBeat && beat < chord.endBeat)
        {
            return chord.name;
        }
    }
    return "N.C";
}

std::string Prompter::getCurrentChord()
{
    return getChordAtBeat(currentBeat);
}

std::vector<std::string> Prompter::getNext8Chord()
{
    std::vector<std::string> nextChords;
    std::string output = "| ";
    for (int i = 1; i <= 8; i++)
    {
        nextChords.push_back(getChordAtBeat(currentBeat + i));
        output += getChordAtBeat(currentBeat + i);
        output += " | ";
    }
    Serial.println(output.c_str());
    return nextChords;
}

void Prompter::chordUpdateCallback()
{
    prompter.updateCurrentBeat();
}

void Prompter::updateCurrentBeat()
{

    Serial.print("Current Bar: ");
    Serial.print(getCurrentBar());
    Serial.print(", Current Beat: ");
    Serial.println(currentBeat);

    Serial.print("Current Chord: ");
    Serial.println(getChordAtBeat(currentBeat).c_str());

    Serial.print("Next 8 Chord: ");
    getNext8Chord();

    currentBeat++;
    if (currentBeat >= musicScore.chords.back().endBeat)
    {
        stop();
    }
}
