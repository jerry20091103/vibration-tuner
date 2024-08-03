#include "MetronomePage.h"
#include "hardware.h"
#include "Metronome.h"

void MetronomePage::onBtnPressed(uint8_t pin)
{
    Serial.println("MetronomePage: onBtnPressed");
    metronome.toggleVibrating();
}

void MetronomePage::onBtnHold(uint8_t pin)
{
    Serial.println("MetronomePage: onBtnHold");
    if (pin == ENC_BTN)
    {
        PageManager.switchPage(PAGE_TUNER, true);
    }
}

void MetronomePage::onBtnDoubleClicked(uint8_t pin)
{
    Serial.println("MetronomePage: onBtnDoubleClicked");
    if (pin == ENC_BTN)
    {
        PageManager.switchPage(PAGE_METRONOME_OPTIONS);
    }
}

void MetronomePage::onEncTurned(int value)
{
    currentBPM = value + 30;
    if (currentBPM > MAX_BPM)
    {
        currentBPM = MAX_BPM;
    }
    else if (currentBPM < MIN_BPM)
    {
        currentBPM = MIN_BPM;
    }
    metronome.setBPM(currentBPM);
    lv_label_set_text_fmt(labelBPM, "BPM: %d", currentBPM);
}

void MetronomePage::update()
{
    // update beat count
    lv_label_set_text_fmt(labelBeat, "Beat: %d/%d", metronome.getCurrentBeat(), metronome.getBeatCount());
}


void MetronomePage::init()
{
    Serial.println("MetronomePage: init");
    pageID = PAGE_METRONOME;
    // init metronome
    metronome.setBPM(currentBPM);
    metronome.setStrongHapticWaveform(HAPTIC_WAVEFORM_CLICK);
    metronome.setWeakHapticWaveform(HAPTIC_WAVEFORM_TICK);
    // create screen
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    // title
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "Metronome");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_26, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
    // BPM label
    labelBPM = lv_label_create(screen);
    lv_label_set_text_fmt(labelBPM, "BPM: %d", currentBPM);
    lv_obj_set_style_text_font(labelBPM, &lv_font_montserrat_48, 0);
    lv_obj_align(labelBPM, LV_ALIGN_CENTER, 0, 0);
    // beat label
    labelBeat = lv_label_create(screen);
    lv_label_set_text_fmt(labelBeat, "Beat: %d/%d", metronome.getCurrentBeat(), metronome.getBeatCount());
    lv_obj_set_style_text_font(labelBeat, &lv_font_montserrat_26, 0);
    lv_obj_align(labelBeat, LV_ALIGN_BOTTOM_MID, 0, -30);
}

void MetronomePage::load()
{
    Serial.println("MetronomePage: load");
    enc0->changePrecision(270, currentBPM-30);
}

void MetronomePage::unload()
{
    Serial.println("MetronomePage: unload");
}


