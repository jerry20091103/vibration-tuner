#include "TunerPage.h"
#include "hardware.h"

void TunerPage::onBtnPressed(uint8_t pin)
{
    Serial.println("TunerPage: onBtnPressed");
}

void TunerPage::onBtnHold(uint8_t pin)
{
    Serial.println("TunerPage: onBtnHold");
    if (pin == ENC_BTN)
    {
        PageManager.switchPage(PAGE_PROMPTER);
    }
}

void TunerPage::onBtnDoubleClicked(uint8_t pin)
{
    Serial.println("TunerPage: onBtnDoubleClicked");
}

void TunerPage::onEncTurned(int value)
{
    Serial.println("TunerPage: onEncTurned");
}

void TunerPage::update()
{
}

void TunerPage::init()
{
    Serial.println("TunerPage: init");
}

void TunerPage::load()
{
    Serial.println("TunerPage: load");
}

void TunerPage::unload()
{
    Serial.println("TunerPage: unload");
}
