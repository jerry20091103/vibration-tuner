#ifndef METRONOME_PAGE_H
#define METRONOME_PAGE_H

#include "pages.h"
#include "Metronome.h"

# define MAX_BPM 300
# define MIN_BPM 30

class MetronomePage : public Pages
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
    // user data
    int currentBPM = 60;
    // lvgl object ref
    lv_obj_t *beatLED[16];
    lv_obj_t *bpmArc;
    // variables
    int lastBeatCount = -1;
};

#endif // METRONOME_PAGE_H