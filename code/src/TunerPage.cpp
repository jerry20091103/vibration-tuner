#include "TunerPage.h"
#include "hardware.h"

void TunerPage::onBtnPressed(uint8_t pin)
{
    Serial.println("TunerPage: onBtnPressed");
}

void TunerPage::onBtnHold(uint8_t pin)
{
    Serial.println("TunerPage: onBtnHold");
    if (pin == ENC_BTN)
    {
        PageManager.switchPage(PAGE_PROMPTER, true);
    }
}

void TunerPage::onBtnDoubleClicked(uint8_t pin)
{
    Serial.println("TunerPage: onBtnDoubleClicked");
}

void TunerPage::onEncTurned(int value)
{
    Serial.println("TunerPage: onEncTurned");
}

void TunerPage::update()
{
}

void TunerPage::init()
{
    Serial.println("TunerPage: init");
    pageID = PAGE_TUNER;
    // create screen
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    // title
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "Tuner");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_26, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
}

void TunerPage::load()
{
    Serial.println("TunerPage: load");
}

void TunerPage::unload()
{
    Serial.println("TunerPage: unload");
}
