#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <IoAbstraction.h>
#include <Button2.h>
#include <Adafruit_DRV2605.h>
#include <lvgl.h>
#include <lv_conf.h>
#include <TFT_eSPI.h>

//* Pins
// encoder pins
#define ENC_A D6
#define ENC_B D7
#define ENC_BTN D3

// haptic driver pins
#define DRV_SDA D4
#define DRV_SCL D5

#define BUZZER_PIN D9
#define PIEZO_PIN D2

// haptic waveform
#define HAPTIC_WAVEFORM_CLICK 1
#define HAPTIC_WAVEFORM_TICK 24

// rotary encoder object
extern HardwareRotaryEncoder *enc0;

// button object for button2 library
extern Button2 btnEnc;
extern Button2 btn0;

// haptic driver object
extern Adafruit_DRV2605 haptic;

void hardwareSetup();
void updateGui();

#endif // HARDWARE_H