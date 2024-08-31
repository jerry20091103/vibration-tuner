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
    currentBPM = 60;

    // load default music score
    musicScore = createDefaultMusicScore();
}

MusicScore Prompter::createDefaultMusicScore()
{
    MusicScore score;
    score.BPM = 120;
    score.beatsPerMeasure = 4;
    // load default chords
    score.chords.push_back({"C", 0, 4});
    score.chords.push_back({"Am", 4, 8});
    score.chords.push_back({"F", 8, 12});
    score.chords.push_back({"G", 12, 16});
    score.chords.push_back({"Em", 16, 18});
    score.chords.push_back({"G", 18, 20});
    score.chords.push_back({"C", 20, 22});
    score.chords.push_back({"Am", 22, 24});
    return score;
}

void Prompter::togglePrompter()
{
    if (isRunning)
    {
        isRunning = false;
        taskManager.cancelTask(prompterTaskID);
        metronome.stopVibrating();
    }
    else
    {
        isRunning = true;
        int period = 60000000 / currentBPM;
        Serial.println("Prompter started.");
        prompterTaskID = taskManager.scheduleFixedRate(
            period, chordUpdateCallback, TIME_MICROS);

        // start metronome
        metronome.setBPM(currentBPM);
        metronome.startVibrating();
    }
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
        isRunning = true;
        int period = 60000000 / currentBPM;
        Serial.println("Prompter started.");

        prompterTaskID = taskManager.scheduleFixedRate(
            period, chordUpdateCallback, TIME_MICROS);

        // start metronome
        metronome.setBPM(currentBPM);
        metronome.startVibrating();
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
        metronome.stopVibrating();
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

int Prompter::getCurrentBar()
{
    return floor((double)currentBeat / musicScore.beatsPerMeasure);
}

void Prompter::setMusicScore(const MusicScore& score)
{
    musicScore = score;
    metronome.setBPM(score.BPM);
    currentBPM = musicScore.BPM;
    isScoreLoaded = true;
    Serial.println("setMusicScore end");
}

String Prompter::readJSONFromSerial()
{
    String readBuffer;
    int braceCount = 0;
    bool isJsonStarted = false;

    while (Serial.available())
    {
        char c = Serial.read();

        if (c == '{')
        {
            if (!isJsonStarted)
                isJsonStarted = true;
            braceCount++;
        }

        if (isJsonStarted)
        {
            readBuffer += c;
        }

        if (c == '}' && isJsonStarted)
        {
            braceCount--;
            if (braceCount == 0)
                break;
        }
    }

    Serial.print("serial reading: ");
    Serial.println(readBuffer);

    if (braceCount == 0 && isJsonStarted)
        return readBuffer;
    else
        return "";
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

    if (doc["BPM"].isNull() || !doc["BPM"].is<int>() ||
        doc["beatsPerMeasure"].isNull() || !doc["beatsPerMeasure"].is<int>() ||
        doc["chords"].isNull() || !doc["chords"].is<JsonArray>() ||
        doc["chords"].size() == 0)
    {
        Serial.println("Error: Music score JSON content is not complete or invalid");
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

void Prompter::loadMusicScoreFromUSBSerial()
{
    String musicScoreJSON = readJSONFromSerial();
    if (musicScoreJSON == "")
        Serial.println("Error: Read json from serial failed, please check the input.");
    else
        loadMusicScoreFromJSON(musicScoreJSON);
}

void Prompter::setSpeed(float speed)
{
    if (speed >= 0.5 && speed <= 2.0)
    {
        speedMultiplier = speed;
        currentBPM = prompter.getBPM() * speed;
        if (isRunning)
        {
            taskManager.cancelTask(prompterTaskID);
            int period = 60000000 / currentBPM;
            prompterTaskID = taskManager.scheduleFixedRate(
                period, chordUpdateCallback, TIME_MICROS);
            // sync metronome
            metronome.setBPM(currentBPM);
        }
        Serial.print("Speed set to: ");
        Serial.println(speedMultiplier);
        Serial.print("current BPM: ");
        Serial.println(currentBPM);
    }
}

float Prompter::getSpeed()
{
    return speedMultiplier;
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

int Prompter::getBeatsPerBar()
{
    return musicScore.beatsPerMeasure;
}

int Prompter::getScoreLength()
{
    return musicScore.chords.back().endBeat;
}

int Prompter::getBPM()
{
    return musicScore.BPM;
}

int Prompter::getCurrentBPM()
{
    return currentBPM;
}

std::vector<std::string> Prompter::getCurrent8Chord()
{
    std::vector<std::string> nextChords;
    std::string output = "| ";
    for (int i = 0; i < 8; i++)
    {
        nextChords.push_back(getChordAtBeat(currentBeat + i));
        output += getChordAtBeat(currentBeat + i);
        output += " | ";
    }
    Serial.println(output.c_str());
    return nextChords;
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

    Serial.print("Current 8 Chord: ");
    getCurrent8Chord();

    currentBeat++;
    if (currentBeat >= musicScore.chords.back().endBeat)
    {
        stop();
    }
}
