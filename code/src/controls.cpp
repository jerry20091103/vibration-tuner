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
    else if (btn == btn0)
    {
        // do something with the second button
        Serial.println("Second button pressed");
        PageManager.getCurPageRef()->onBtnPressed(SEC_BTN);
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
    else if (btn == btn0)
    {
        // do something with the second button
        Serial.println("Second button held");
        PageManager.getCurPageRef()->onBtnHold(SEC_BTN);
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
    else if (btn == btn0)
    {
        // do something with the second button
        Serial.println("Second button double clicked");
        PageManager.getCurPageRef()->onBtnDoubleClicked(SEC_BTN);
    }
}

void Enc0Callback(int value)
{
    // do something with the encoder value
    Serial.printf("Encoder value: %d\n", value);
    PageManager.getCurPageRef()->onEncTurned(value);
}