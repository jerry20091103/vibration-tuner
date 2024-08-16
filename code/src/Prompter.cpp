#include "Metronome.h"
#include "Prompter.h"

Prompter prompter;

Prompter::Prompter()
{
    currentBeat = 0;
    currentBar = 0;
    speedMultiplier = 1.0;
    isRunning = false;
    displayTaskID = -1;
}

void Prompter::start() {
    if (!isRunning) {
        Serial.println("Prompter started.");
        isRunning = true;
        int period = 60000000 / (int)(metronome.getBPM() * speedMultiplier);
        displayTaskID = taskManager.scheduleFixedRate(period, chordUpdateCallback, TIME_MICROS);
    }
}

void Prompter::stop() {
    if (isRunning) {
        Serial.println("Prompter stopped.");
        isRunning = false;
        taskManager.cancelTask(displayTaskID);
    }
}

void Prompter::setCurrentBar(int bar) {
    currentBar = bar;
    currentBeat = bar * musicScore.beatsPerMeasure;
    Serial.print("Current bar set to: ");
    Serial.println(currentBar);
}

int Prompter::getCurrentBar() {
    return currentBar;
}

void Prompter::setMusicScore(const MusicScore& score) {
    musicScore = score;
    Serial.println(musicScore.beatsPerMeasure);
    Serial.println("Music score set.");
}

void Prompter::loadMusicScoreFromJSON(const String& json) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }

    MusicScore score;
    score.beatsPerMeasure = doc["beatsPerMeasure"];

    JsonArray chords = doc["chords"];
    for (JsonObject chord : chords) {
        Chord newChord;
        newChord.name = chord["name"].as<std::string>();
        newChord.startBeat = chord["startBeat"];
        newChord.endBeat = chord["endBeat"];
        score.chords.push_back(newChord);
    }

    setMusicScore(score);
    Serial.println("Music score loaded from JSON and set.");
}

void Prompter::setSpeed(float speed) {
    if (speed >= 0.5 && speed <= 2.0) {
        speedMultiplier = speed;
        Serial.print("Speed set to: ");
        Serial.println(speedMultiplier);
    }
}

std::string Prompter::getCurrentChord() {
    int index = findChordIndexByBeat(currentBeat);
    if (index != -1) {
        Serial.print("Current chord: ");
        Serial.println(musicScore.chords[index].name.c_str());
        return musicScore.chords[index].name;
    }
    return "";
}

std::string Prompter::getNextChord() {
    int index = findChordIndexByBeat(currentBeat);
    if (index != -1 && index + 1 < musicScore.chords.size()) {
        Serial.print("Next chord: ");
        Serial.println(musicScore.chords[index + 1].name.c_str());
        return musicScore.chords[index + 1].name;
    }
    return "";
}

std::string Prompter::getNextNextChord() {
    int index = findChordIndexByBeat(currentBeat);
    if (index != -1 && index + 2 < musicScore.chords.size()) {
        Serial.print("Next next chord: ");
        Serial.println(musicScore.chords[index + 2].name.c_str());
        return musicScore.chords[index + 2].name;
    }
    return "";
}

int Prompter::findChordIndexByBeat(int beat) {
    for (int i = 0; i < musicScore.chords.size(); i++) {
        if (musicScore.chords[i].startBeat <= beat && musicScore.chords[i].endBeat > beat) {
            return i;
        }
    }
    return -1;
}

void Prompter::chordUpdateCallback() {
    prompter.updateCurrentBeat(); 
}

void Prompter::updateCurrentBeat() {
    currentBeat++;
    Serial.print("musicScore.beatsPerMeasure: ");
    Serial.println(musicScore.beatsPerMeasure);

    if (currentBeat >= musicScore.beatsPerMeasure) {
        currentBeat = 0;
        currentBar++; 
    }

    Serial.print("Current Bar: ");
    Serial.print(currentBar);
    Serial.print(", Current Beat: ");
    Serial.println(currentBeat);
    getCurrentChord();
}
