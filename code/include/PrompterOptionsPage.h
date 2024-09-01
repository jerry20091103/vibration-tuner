#ifndef PROMPTER_OPTIONS_PAGE_H
#define PROMPTER_OPTIONS_PAGE_H

#include "pages.h"

class PrompterOptionsPage : public Pages
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
    float speed = 1.0f;
    // lvgl objects
    lv_group_t *listGroup;
    lv_obj_t *listItem[5];
    // variables
    bool isAdjusting = false;
};

#endif // PROMPTER_OPTIONS_PAGE_H