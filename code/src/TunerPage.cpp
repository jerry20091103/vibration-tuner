#include "TunerPage.h"
#include "hardware.h"
#include "guiUtility.h"
#include "Tuner.h"

LV_IMG_DECLARE(tuner_icon);

// tuner colors
static const lv_color_t color_Red = lv_color_make(255, 0, 0);
static const lv_color_t color_RedDark = lv_color_make(50, 0, 0);
static const lv_color_t color_Green = lv_color_make(0, 255, 0);
static const lv_color_t color_GreenDark = lv_color_make(0, 40, 0);

void TunerPage::onBtnPressed(uint8_t pin)
{
}

void TunerPage::onBtnHold(uint8_t pin)
{
    if (pin == ENC_BTN)
    {
        PageManager.switchPage(PAGE_PROMPTER, true);
    }
}

void TunerPage::onBtnDoubleClicked(uint8_t pin)
{
}

void TunerPage::onEncTurned(int value)
{
    // change string to tune
    stringId += value;
    stringId = constrain(stringId, 0, 5);
    lv_label_set_text_fmt(getArcValueLabel(stringArc), "%d", stringId + 1);
    lv_arc_set_value(stringArc, stringId);
    tuner.setStringToCalculate(stringId);
    // set note name
    switch (stringId)
    {
    case 0:
        lv_label_set_text(noteLabel, "E2");
        break;
    case 1:
        lv_label_set_text(noteLabel, "A2");
        break;
    case 2:
        lv_label_set_text(noteLabel, "D3");
        break;
    case 3:
        lv_label_set_text(noteLabel, "G3");
        break;
    case 4:
        lv_label_set_text(noteLabel, "B3");
        break;
    case 5:
        lv_label_set_text(noteLabel, "E4");
        break;
    default:
        break;
    }
}

void TunerPage::update()
{
    // test cent display
    std::vector<float> freq = tuner.getFrequncy();
    std::vector<float> expectedFreq = tuner.getExpectedFrequencies();
    // 0 string cent
    int cent = 1200 * log2(freq[stringId] / expectedFreq[stringId]);

    updateTunerCent(cent);
}

void TunerPage::init()
{
    Serial.println("TunerPage: init");
    pageID = PAGE_TUNER;
    // create screen
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    // leds
    for (int i = 0; i < 3; i++)
    {
        tuneLED[i] = lv_obj_create(screen);
        lv_obj_set_size(tuneLED[i], 40, 40);
        lv_obj_set_style_radius(tuneLED[i], LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_border_width(tuneLED[i], 0, 0);
    }
    lv_obj_align(tuneLED[0], LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_align(tuneLED[1], LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_align(tuneLED[2], LV_ALIGN_TOP_RIGHT, -5, 5);
    setTunerLED(false, false, false);
    // indicator bar
    indicatorBar = lv_bar_create(screen);
    lv_bar_set_range(indicatorBar, -50, 50);
    lv_bar_set_mode(indicatorBar, LV_BAR_MODE_SYMMETRICAL);
    lv_obj_set_size(indicatorBar, 310, 40);
    lv_obj_align(indicatorBar, LV_ALIGN_TOP_MID, 0, 60);
    // note name
    noteLabel = lv_label_create(screen);
    lv_label_set_text(noteLabel, "A4");
    lv_obj_set_style_text_font(noteLabel, &lv_font_montserrat_48, 0);
    lv_obj_align(noteLabel, LV_ALIGN_TOP_MID, 0, 120);
    // string arc
    stringArc = createArc(screen, LV_PALETTE_LIGHT_GREEN, "No.", 100, 10);
    lv_obj_align(stringArc, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_t *valueLabel = getArcValueLabel(stringArc);
    lv_obj_set_style_text_font(valueLabel, &lv_font_montserrat_24, 0);
    lv_label_set_text_fmt(valueLabel, "%d", stringId + 1);
    lv_arc_set_range(stringArc, 0, 5);
    lv_arc_set_value(stringArc, stringId);
    lv_obj_t *unitLabel = getArcUnitLabel(stringArc);
    lv_obj_set_style_text_font(unitLabel, &lv_font_montserrat_20, 0);
    centLabel = lv_label_create(screen);
    lv_label_set_text(centLabel, "+0 cent");
    lv_obj_align(centLabel, LV_ALIGN_BOTTOM_MID, 0, -10);

    // icon
    createModeIcon(screen, &tuner_icon, LV_PALETTE_LIGHT_GREEN);
}

void TunerPage::load()
{
    // use direction mode
    enc0->changePrecision(0, 0);
    // set tuner user data
    // tuner.setReferenceFrequency(a4Freq);
    tuner.setStringToCalculate(stringId);
    tuner.start();
}

void TunerPage::unload()
{
    tuner.stop();
}

void TunerPage::updateTunerCent(int cent)
{
    // cent label
    if (cent >= 0)
        lv_label_set_text_fmt(centLabel, "+%d cent", cent);
    else
        lv_label_set_text_fmt(centLabel, "%d cent", cent);
    // led
    if (cent < -10)
    {
        setTunerLED(true, false, false);
    }
    else if (cent < -2)
    {
        setTunerLED(true, true, false);
    }
    else if (cent < 2)
    {
        setTunerLED(false, true, false);
    }
    else if (cent < 10)
    {
        setTunerLED(false, true, true);
    }
    else 
    {
        setTunerLED(false, false, true);
    }
    // indicator bar
    lv_bar_set_value(indicatorBar, cent, LV_ANIM_OFF);
    // gradient color depend on cent value
    // set color from 0~1
    float gradient = abs(cent) * 0.02;
    float red = constrain(2 * gradient, 0, 1);
    float green = constrain(2 * (1 - gradient), 0, 1);
    lv_color_t color = lv_color_make(red * 255, green * 255, 0);
    lv_color_t colorDark = lv_color_make(red * 255 * 0.6, green * 255 * 0.6, 0);
    lv_obj_set_style_bg_color(indicatorBar, color, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(indicatorBar, colorDark, LV_PART_MAIN);
}

void TunerPage::setTunerLED(bool leftOn, bool midOn, bool rightOn)
{
    if (leftOn != lastTuneLED[0])
    {
        lastTuneLED[0] = leftOn;
        lv_obj_set_style_bg_color(tuneLED[0], (leftOn) ? color_Red : color_RedDark, 0);
    }
    if (midOn != lastTuneLED[1])
    {
        lastTuneLED[1] = midOn;
        lv_obj_set_style_bg_color(tuneLED[1], (midOn) ? color_Green : color_GreenDark, 0);
    }
    if (rightOn != lastTuneLED[2])
    {
        lastTuneLED[2] = rightOn;
        lv_obj_set_style_bg_color(tuneLED[2], (rightOn) ? color_Red : color_RedDark, 0);
    }
}
