#ifndef CONTROLS_H
#define CONTROLS_H

#include "Hardware.h"

// wraps the library callbacks into gui callbacks
void BtnPressCallback(Button2 &btn);
void BtnHoldCallback(Button2 &btn);
void BtnDoubleClickCallback(Button2 &btn);
void Enc0Callback(int value);

#endif