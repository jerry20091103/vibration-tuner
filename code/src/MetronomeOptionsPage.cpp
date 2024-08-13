#include "MetronomeOptionsPage.h"
#include "Metronome.h"
#include "guiUtility.h"

void MetronomeOptionsPage::onBtnPressed(uint8_t pin)
{
    if (isAdjusting)
    {
        isAdjusting = false;
        lv_obj_t *valueLabel = getOptionListItemValueLabel(lv_group_get_focused(listGroup));
        // set checked state
        lv_obj_remove_state(getOptionListItemValueLabel(lv_group_get_focused(listGroup)), LV_STATE_CHECKED);
        // remove left and right symbol
        String valueStr = lv_label_get_text(valueLabel);
        lv_label_set_text(valueLabel, removeAdjustingText(valueStr));
    }
    else
    {
        isAdjusting = true;
        lv_obj_t *valueLabel = getOptionListItemValueLabel(lv_group_get_focused(listGroup));
        // set checked state
        lv_obj_add_state(valueLabel, LV_STATE_CHECKED);
        // add left and right symbol
        String valueStr = lv_label_get_text(valueLabel);
        lv_label_set_text(valueLabel, createAdjustingText(valueStr));
    }
}

void MetronomeOptionsPage::onBtnHold(uint8_t pin)
{
}

void MetronomeOptionsPage::onBtnDoubleClicked(uint8_t pin)
{
    if (!isAdjusting)
    {
        PageManager.switchPage(PAGE_METRONOME);
    }
}

void MetronomeOptionsPage::onEncTurned(int value)
{
    if (isAdjusting)
    {
        if (lv_group_get_focused(listGroup) == listItem[0])
        {

            beatCount += value;
            beatCount = constrain(beatCount, 1, 16);
            lv_label_set_text(getOptionListItemValueLabel(listItem[0]), createAdjustingText(String(beatCount)));
            metronome.setBeatCount(beatCount);
        }
        else if (lv_group_get_focused(listGroup) == listItem[1])
        {
            strongHapticMode += value;
            strongHapticMode = constrain(strongHapticMode, 0, MAX_HAPTIC_MODES - 1);
            lv_label_set_text(getOptionListItemValueLabel(listItem[1]), createAdjustingText(hapticModeStr[strongHapticMode]));
            setHapticMode(strongHapticMode, hapticLevel, true);
        }
        else if (lv_group_get_focused(listGroup) == listItem[2])
        {
            weakHapticMode += value;
            weakHapticMode = constrain(weakHapticMode, 0, MAX_HAPTIC_MODES - 1);
            lv_label_set_text(getOptionListItemValueLabel(listItem[2]), createAdjustingText(hapticModeStr[weakHapticMode]));
            setHapticMode(weakHapticMode, hapticLevel, false);
        }
        else if (lv_group_get_focused(listGroup) == listItem[3])
        {
            buzzerLevel += value;
            buzzerLevel = constrain(buzzerLevel, 0, 10);
            lv_label_set_text(getOptionListItemValueLabel(listItem[3]), createAdjustingText(String(buzzerLevel)));
            // metronome.setBuzzerLevel(buzzerLevel);
        }
        else if (lv_group_get_focused(listGroup) == listItem[4])
        {
            hapticLevel -= value;
            hapticLevel = constrain(hapticLevel, 0, 2);
            lv_label_set_text(getOptionListItemValueLabel(listItem[4]), createAdjustingText(hapticLevelStr[hapticLevel]));
            setHapticMode(strongHapticMode, hapticLevel, true);
            setHapticMode(weakHapticMode, hapticLevel, false);
        }
    }
    else
    {
        if (value > 0)
        {
            lv_group_focus_next(listGroup);
        }
        else
        {
            lv_group_focus_prev(listGroup);
        }
    }
}

void MetronomeOptionsPage::update()
{
}

void MetronomeOptionsPage::init()
{
    pageID = PAGE_METRONOME;
    // create screen
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    // create title
    lv_obj_t *titleArea = lv_obj_create(screen);
    lv_obj_set_size(titleArea, 320, 55);
    lv_obj_align(titleArea, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_remove_flag(titleArea, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t *title = lv_label_create(titleArea);
    lv_label_set_text(title, "Metronome Options");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    // list
    lv_obj_t *list = lv_list_create(screen);
    lv_obj_set_size(list, 320, 185);
    lv_obj_set_pos(list, 0, 55);

    listGroup = lv_group_create();

    listItem[0] = createOptionListItem(list, LV_SYMBOL_AUDIO, "Beats per measure", String(beatCount).c_str());
    listItem[1] = createOptionListItem(list, LV_SYMBOL_SETTINGS, "Primary haptic waveform", hapticModeStr[strongHapticMode]);
    listItem[2] = createOptionListItem(list, LV_SYMBOL_SETTINGS, "Secondary haptic waveform", hapticModeStr[weakHapticMode]);
    listItem[3] = createOptionListItem(list, LV_SYMBOL_VOLUME_MAX, "Buzzer volume", "10");
    listItem[4] = createOptionListItem(list, LV_SYMBOL_SETTINGS, "Haptic intensity", "Strong");

    for (int i = 0; i < 5; i++)
    {
        lv_group_add_obj(listGroup, listItem[i]);
    }
    lv_group_focus_obj(listItem[0]);
}

void MetronomeOptionsPage::load()
{
    enc0->changePrecision(0, 0); // use direction mode
    // set metronome options
    metronome.setBeatCount(beatCount);
}

void MetronomeOptionsPage::unload()
{
}

void MetronomeOptionsPage::setHapticMode(int mode, int level, bool isStrong)
{
    int waveform = 0;
    switch (mode)
    {
    case 0: // off
        waveform = 0;
        break;
    case 1: // Click
        waveform = 17;
        break;
    case 2: // Tick
        waveform = 24;
        break;
    case 3: // Bump
        waveform = 7;
        break;
    }
    if (isStrong)
    {
        metronome.setStrongHapticWaveform(waveform + level);
    }
    else
    {
        metronome.setWeakHapticWaveform(waveform + level);
    }
}
