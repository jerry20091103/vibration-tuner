#include "Hardware.h"
#include "controls.h"

HardwareRotaryEncoder *enc0;

Button2 btnEnc;
Button2 btn0;

void hardwareSetup() {
    // setup rotary encoder
    switches.initialiseInterrupt(ioUsingArduino(), true);
    enc0 = new HardwareRotaryEncoder(ENC_A, ENC_B, Enc0Callback);
    switches.setEncoder(0, enc0);
    enc0->changePrecision(0, 0); // use encoder in direction mode

    btnEnc.begin(ENC_BTN);
    btnEnc.setClickHandler(BtnPressCallback);
    btnEnc.setLongClickHandler(BtnHoldCallback);
    btnEnc.setDoubleClickHandler(BtnDoubleClickCallback);

    // setup second button
    btn0.begin(SEC_BTN);
    btn0.setClickHandler(BtnPressCallback);
    btn0.setLongClickHandler(BtnHoldCallback);
    btn0.setDoubleClickHandler(BtnDoubleClickCallback);
}