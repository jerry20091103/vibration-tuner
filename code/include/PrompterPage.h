#ifndef PROMPTER_PAGE_H
#define PROMPTER_PAGE_H

#include "pages.h"

class PrompterPage : public Pages
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
    int currentBeat = 0;
    // lvgl objects
    lv_obj_t *chordLabel[8];
    lv_obj_t *barLine[7];
    lv_obj_t *positionArc;
    lv_obj_t *bpmLabel;
    lv_obj_t *speedLabel;
    // variables
    int lastBeat = -1;
    lv_point_precise_t barLinePos[7][2] = {
        {{135, 33}, {250, 33}},
        {{135, 67}, {250, 67}},
        {{135, 101}, {250, 101}},
        {{135, 135}, {250, 135}},
        {{135, 169}, {250, 169}},
        {{135, 203}, {250, 203}},
        {{135, 237}, {250, 237}}
    };
};

#endif // PROMPTER_PAGE_H