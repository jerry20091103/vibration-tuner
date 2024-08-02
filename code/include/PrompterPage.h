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
    // add your variables here
};

#endif // PROMPTER_PAGE_H