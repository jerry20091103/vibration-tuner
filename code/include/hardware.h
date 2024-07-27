#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include <IoAbstraction.h>
#include <Button2.h>

//* Pins
// encoder pins
#define ENC_A D1
#define ENC_B D2
#define ENC_BTN D3

// haptic driver pins
#define DRV_SDA D4
#define DRV_SCL D5

// display pins
#define DISP_MOSI D10
#define DISP_MISO D9
#define DISP_SCK D8

#define BUZZER_PIN D7
#define PIEZO_PIN A0
#define SEC_BTN D6

// rotary encoder object
extern HardwareRotaryEncoder *enc0;

// button object for button2 library
extern Button2 btnEnc;
extern Button2 btn0;

void hardwareSetup();

#endif // HARDWARE_H