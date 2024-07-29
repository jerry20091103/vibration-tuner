#include "Hardware.h"
#include "controls.h"


HardwareRotaryEncoder *enc0;

Button2 btnEnc;

Adafruit_DRV2605 haptic;

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

    // setup haptic driver
    if (!Wire.begin(DRV_SDA, DRV_SCL, 400000))
    {
        Serial.println("Failed to start I2C bus");
    }

    // if (!haptic.begin()) {
    //     Serial.println("Failed to start haptic driver");
    // }
    // haptic.selectLibrary(6);
    // haptic.setMode(DRV2605_MODE_INTTRIG);
    // haptic.useLRA();
    // // test first waveform
    // haptic.setWaveform(0, 24);
    // haptic.setWaveform(1, 0);
}