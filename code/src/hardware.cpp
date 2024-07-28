#include "Hardware.h"
#include "controls.h"

HardwareRotaryEncoder *enc0;

Button2 btnEnc;

void hardwareSetup() {
    // setup rotary encoder
    Serial.println("Setting up hardware...");
    switches.init(ioUsingArduino(), SWITCHES_NO_POLLING, INPUT_PULLUP);
    enc0 = new HardwareRotaryEncoder(ENC_A, ENC_B, Enc0Callback);
    switches.setEncoder(0, enc0);
    enc0->changePrecision(0, 0); // use encoder in direction mode

    btnEnc.begin(ENC_BTN);
    btnEnc.setClickHandler(BtnPressCallback);
    btnEnc.setLongClickHandler(BtnHoldCallback);
    btnEnc.setDoubleClickHandler(BtnDoubleClickCallback);
}