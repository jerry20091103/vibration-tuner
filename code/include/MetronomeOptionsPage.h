#ifndef METRONOME_OPTIONS_PAGE_H
#define METRONOME_OPTIONS_PAGE_H

#include "pages.h"

class MetronomeOptionsPage : public Pages
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
    static void setHapticMode(int mode, int level, bool isStrong);

    int beatCount = 4;
    int strongHapticMode = 1;
    int weakHapticMode = 2;
    int hapticLevel = 0;
    int buzzerLevel = 10;
    lv_group_t *listGroup;
    lv_obj_t *listItem[5];
    bool isAdjusting = false;
    const char *hapticModeStr[4] = {"Off", "Click", "Tick", "Bump"};
    const char *hapticLevelStr[4] = {"Strong", "Medium", "Weak"};
    const int MAX_HAPTIC_MODES = 4;
};

#endif // METRONOME_OPTIONS_PAGE_H