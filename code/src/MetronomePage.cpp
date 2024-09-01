#include "MetronomePage.h"
#include "hardware.h"
#include "Metronome.h"
#include "guiUtility.h"

LV_IMG_DECLARE(metronome_icon);

void MetronomePage::onBtnPressed(uint8_t pin)
{
    metronome.toggleVibrating();
}

void MetronomePage::onBtnHold(uint8_t pin)
{
    if (pin == ENC_BTN)
    {
        metronome.stopVibrating();
        PageManager.switchPage(PAGE_TUNER, true);
    }
}

void MetronomePage::onBtnDoubleClicked(uint8_t pin)
{
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
    lv_label_set_text_fmt(getArcValueLabel(bpmArc), "%d", currentBPM);
    lv_arc_set_value(bpmArc, currentBPM);
}

void MetronomePage::update()
{
    if (lastBeatCount != metronome.getCurrentBeat())
    {
        lastBeatCount = metronome.getCurrentBeat();
        // update beat led
        for (int i = 0; i < metronome.getBeatCount(); i++)
        {
            if (i == metronome.getCurrentBeat())
            {
                lv_obj_set_style_bg_color(beatLED[i], lv_palette_lighten(LV_PALETTE_LIGHT_BLUE, 4), 0);
            }
            else
            {
                lv_obj_set_style_bg_color(beatLED[i], lv_palette_darken(LV_PALETTE_LIGHT_BLUE, 4), 0);
            }
        }
    }
}


void MetronomePage::init()
{
    pageID = PAGE_METRONOME;
    // create screen
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    // beat led area
    lv_obj_t *ledArea = lv_obj_create(screen);
    lv_obj_set_size(ledArea, 320, 50);
    lv_obj_set_style_bg_color(ledArea, lv_color_black(), 0);
    lv_obj_set_style_border_color(ledArea, lv_color_black(), 0);
    lv_obj_set_style_pad_all(ledArea, 0, 0);
    lv_obj_align(ledArea, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_layout(ledArea, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(ledArea, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ledArea, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(ledArea, LV_OBJ_FLAG_SCROLLABLE);
    // add some led
    for (int i = 0; i < 16; i++)
    {
        beatLED[i] = lv_obj_create(ledArea);
        lv_obj_set_size(beatLED[i], LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_style_bg_color(beatLED[i], lv_palette_darken(LV_PALETTE_LIGHT_BLUE, 4), 0);
        lv_obj_set_style_radius(beatLED[i], LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_pad_all(beatLED[i], 0, 0);
        lv_obj_set_style_border_width(beatLED[i], 0, 0);
    }
    // BPM arc
    bpmArc = createArc(screen, LV_PALETTE_LIGHT_BLUE, "BPM", 170, 12);
    lv_obj_align(bpmArc, LV_ALIGN_CENTER, 0, 20);
    lv_obj_t *valueLabel = getArcValueLabel(bpmArc);
    lv_obj_set_style_text_font(valueLabel, &lv_font_montserrat_48, 0);
    lv_label_set_text_fmt(valueLabel, "%d", currentBPM);
    lv_arc_set_range(bpmArc, MIN_BPM, MAX_BPM);
    lv_arc_set_value(bpmArc, currentBPM);
    lv_obj_t *unitLabel = getArcUnitLabel(bpmArc);
    lv_obj_set_style_text_font(unitLabel, &lv_font_montserrat_24, 0);
    // icon
    createModeIcon(screen, &metronome_icon, LV_PALETTE_LIGHT_BLUE);
}

void MetronomePage::load()
{
    enc0->changePrecision(270, currentBPM-30);
    // init metronome
    metronome.setBPM(currentBPM);
    // hide some led
    for (int i = 0; i < metronome.getBeatCount(); i++)
    {
        lv_obj_remove_flag(beatLED[i], LV_OBJ_FLAG_HIDDEN);
    }
    for (int i = metronome.getBeatCount(); i < 16; i++)
    {
        lv_obj_add_flag(beatLED[i], LV_OBJ_FLAG_HIDDEN);
    }
    if (metronome.getBeatCount() > 11)
    {
        // shrink led size
        for (int i = 0; i < 16; i++)
        {
            lv_obj_set_size(beatLED[i], 10 ,30);
        }
    }
    else if (metronome.getBeatCount() > 8)
    {
        // shrink led size
        for (int i = 0; i < 16; i++)
        {
            lv_obj_set_size(beatLED[i], 20, 30);
        }
    }
    else 
    {
        // normal led size
        for (int i = 0; i < 16; i++)
        {
            lv_obj_set_size(beatLED[i], 30, 30);
        }
    }
    // set last beat count to force update at first load!
    lastBeatCount = -1;
}

void MetronomePage::unload()
{
}


