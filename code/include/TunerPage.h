#ifndef TUNER_PAGE_H
#define TUNER_PAGE_H

#include "pages.h"

class TunerPage : public Pages
{
public:
    void onBtnPressed(uint8_t pin);
    void onBtnHold(uint8_t pin);
    void onBtnDoubleClicked(uint8_t pin);
    void onEncTurned(int value);
    void update();
    void init();
    void load();
    void unload();
private:
    void updateTunerCent(int cent);
    void setTunerLED(bool leftOn, bool midOn, bool rightOn);
    // user data
    int a4Freq = 440;
    // lvgl object ref
    lv_obj_t *tuneLED[3];
    lv_obj_t *refFreqArc;
    lv_obj_t *indicatorBar;
    lv_obj_t *noteLabel;
    lv_obj_t *freqLabel;
    lv_obj_t *centLabel;
    // variables
    int testCent = 0;
    bool lastTuneLED[3] = {false, false, false};
};

#endif // TUNER_PAGE_H