#include "MetronomePage.h"
#include "hardware.h"
#include "Metronome.h"

void MetronomePage::onBtnPressed(uint8_t pin)
{
    Serial.println("MetronomePage: onBtnPressed");
    metronome.toggleVibrating();
}

void MetronomePage::onBtnHold(uint8_t pin)
{
    Serial.println("MetronomePage: onBtnHold");
    if (pin == ENC_BTN)
    {
        PageManager.switchPage(PAGE_TUNER);
    }
}

void MetronomePage::onBtnDoubleClicked(uint8_t pin)
{
    Serial.println("MetronomePage: onBtnDoubleClicked");
}

void MetronomePage::onEncTurned(int value)
{
    Serial.println("MetronomePage: onEncTurned");
    metronome.updateBPM(value);
}

void MetronomePage::update()
{
}


void MetronomePage::init()
{
    Serial.println("MetronomePage: init");
}

void MetronomePage::load()
{
    Serial.println("MetronomePage: load");
}

void MetronomePage::unload()
{
    Serial.println("MetronomePage: unload");
}


