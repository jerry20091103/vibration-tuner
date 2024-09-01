#include "PrompterOptionsPage.h"
#include "Prompter.h"
#include "guiUtility.h"
#include "hardware.h"

void PrompterOptionsPage::onBtnPressed(uint8_t pin)
{
    // if USB transfer option is selected
    if (lv_group_get_focused(listGroup) == listItem[1])
    {
        // show a message box
        lv_obj_t *msgBox = lv_msgbox_create(NULL);
        lv_obj_set_size(msgBox, 270, 190);
        lv_obj_t *title = lv_msgbox_add_title(msgBox, "USB Transfer");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
        lv_obj_t *text = lv_msgbox_add_text(msgBox, "Please connect via USB and send the music score with the editor program.");
        lv_obj_set_style_text_font(text, &lv_font_montserrat_20, 0);
        // load music score from USB
        // prompter.loadMusicScoreFromUSBSerial();
        taskManager.yieldForMicros(3000000); // todo: replace this with a function that waits USB serial data
        lv_msgbox_close(msgBox);
    }
    else
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
}

void PrompterOptionsPage::onBtnHold(uint8_t pin)
{
}

void PrompterOptionsPage::onEncTurned(int value)
{
    if (isAdjusting)
    {
        if (lv_group_get_focused(listGroup) == listItem[0])
        {
            speed += value * 0.1f;
            speed = constrain(speed, 0.5f, 2.0f);
            lv_label_set_text(getOptionListItemValueLabel(listItem[0]), createAdjustingText(String(speed, 1)));
            prompter.setSpeed(speed);
        }
    }
    else
    {
        if (value > 0)
        {
            lv_group_focus_next(listGroup);
        }
        else if (value < 0)
        {
            lv_group_focus_prev(listGroup);
        }
    }
}

void PrompterOptionsPage::onBtnDoubleClicked(uint8_t pin)
{
    if (!isAdjusting)
    {
        PageManager.switchPage(PAGE_PROMPTER);
    }
}

void PrompterOptionsPage::init()
{
    pageID = PAGE_PROMPTER_OPTIONS;
    // create screen
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    // create title
    lv_obj_t *titleArea = lv_obj_create(screen);
    lv_obj_set_size(titleArea, 320, 55);
    lv_obj_align(titleArea, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_remove_flag(titleArea, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t *title = lv_label_create(titleArea);
    lv_label_set_text(title, "Prompter Options");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    // list
    lv_obj_t *list = lv_list_create(screen);
    lv_obj_set_size(list, 320, 185);
    lv_obj_set_pos(list, 0, 55);

    listGroup = lv_group_create();

    listItem[0] = createOptionListItem(list, LV_SYMBOL_PLAY, "Speed", String(speed, 1).c_str(), LV_PALETTE_DEEP_ORANGE);
    listItem[1] = createOptionListItem(list, LV_SYMBOL_USB, "Get score from USB", "", LV_PALETTE_DEEP_ORANGE);

    for (int i = 0; i < 2; i++)
    {
        lv_group_add_obj(listGroup, listItem[i]);
    }

    lv_group_focus_obj(listItem[0]);
}

void PrompterOptionsPage::load()
{
    // use direction mode
    enc0->changePrecision(0, 0);
    // set prompter options
    prompter.setSpeed(speed);
}

void PrompterOptionsPage::unload()
{
}

void PrompterOptionsPage::update()
{
}
