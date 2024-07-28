#include "controls.h"
#include "pages.h"

void BtnPressCallback(Button2 &btn)
{
    if (btn == btnEnc)
    {
        // do something with the encoder button
        Serial.println("Encoder button pressed");
        PageManager.getCurPageRef()->onBtnPressed(ENC_BTN);
    }
}

void BtnHoldCallback(Button2 &btn)
{
    if (btn == btnEnc)
    {
        // do something with the encoder button
        Serial.println("Encoder button held");
        PageManager.getCurPageRef()->onBtnHold(ENC_BTN);
    }
}

void BtnDoubleClickCallback(Button2 &btn)
{
    if (btn == btnEnc)
    {
        // do something with the encoder button
        Serial.println("Encoder button double clicked");
        PageManager.getCurPageRef()->onBtnDoubleClicked(ENC_BTN);
    }
}

void Enc0Callback(int value)
{
    // do something with the encoder value
    Serial.printf("Encoder value: %d\n", value);
    PageManager.getCurPageRef()->onEncTurned(value);
}