#include "Metronome.h"
#include "Prompter.h"

Prompter prompter;

Prompter::Prompter() {
    currentBeat = 0;
    currentBar = 0;
    currentChord = 0;
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
    getCurrentChord();
}

int Prompter::getCurrentBar() {
    return currentBar;
}

void Prompter::setMusicScore(const MusicScore& score) {
    musicScore = score;
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

    musicScore.beatsPerMeasure = doc["beatsPerMeasure"];
    JsonArray bars = doc["musicScore"];
    for (JsonObject bar : bars) {
        Bar newBar;
        for (JsonObject chord : bar["chords"].as<JsonArray>()) {
            Chord newChord;
            newChord.name = chord["name"].as<std::string>();
            newChord.startBeat = chord["startBeat"];
            newChord.endBeat = chord["endBeat"];
            newBar.chords.push_back(newChord);
        }
        musicScore.bars.push_back(newBar);
    }

    setMusicScore(musicScore);
}

void Prompter::setSpeed(float speed)
{
    if (speed >= 0.5 && speed <= 2.0) {
        speedMultiplier = speed;
        Serial.print("Speed set to: ");
        Serial.println(speedMultiplier);
    }
}

std::string Prompter::getCurrentChord() {
    if (currentBar >= musicScore.bars.size()) {
        Serial.println("getCurrentChord: | No more chords |");
        return "| No more chords |";
    }

    const Bar& currentBarChords = musicScore.bars[currentBar];
    std::string output = "| ";
    for (const Chord& chord : currentBarChords.chords) {
        output += chord.name + " | ";
    }
    Serial.print("getCurrentChord: ");
    Serial.println(output.c_str());
    return output;
}


std::string Prompter::getNextChord() {
    int nextBarIndex = currentBar + 1;
    if (nextBarIndex >= musicScore.bars.size()) {
        Serial.println("getNextChord: | No more chords |");
        return "| No more chords |";
    }

    const Bar& nextBar = musicScore.bars[nextBarIndex];
    std::string output = "| ";
    for (const Chord& chord : nextBar.chords) {
        output += chord.name + " | ";
    }
    Serial.print("getNextChord: ");
    Serial.println(output.c_str());
    return output;
}


std::string Prompter::getNextNextChord() {
    int nextNextBarIndex = currentBar + 2;
    if (nextNextBarIndex >= musicScore.bars.size()) {
        Serial.println("getNextNextChord: | No more chords |");
        return "| No more chords |";
    }

    const Bar& nextNextBar = musicScore.bars[nextNextBarIndex];
    std::string output = "| ";
    for (const Chord& chord : nextNextBar.chords) {
        output += chord.name + " | ";
    }
    Serial.print("getNextNextChord: ");
    Serial.println(output.c_str());
    return output;
}


void Prompter::chordUpdateCallback() {
    prompter.updateCurrentBeat(); 
}

void Prompter::updateCurrentBeat() {
    currentBeat++;
    if (currentBeat >= musicScore.beatsPerMeasure) {
        currentBeat = 0;
        currentBar++; 
        getCurrentChord();
    }
    Serial.print("Current Bar: ");
    Serial.print(currentBar);
    Serial.print(", Current Beat: ");
    Serial.println(currentBeat);
}
