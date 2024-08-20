#include "PrompterPage.h"
#include "hardware.h"
#include "guiUtility.h"

LV_IMG_DECLARE(prompter_icon);

void PrompterPage::onBtnPressed(uint8_t pin)
{
    Serial.println("PrompterPage: onBtnPressed");
}

void PrompterPage::onBtnHold(uint8_t pin)
{
    Serial.println("PrompterPage: onBtnHold");
    if (pin == ENC_BTN)
    {
        PageManager.switchPage(PAGE_METRONOME, true);
    }
}

void PrompterPage::onBtnDoubleClicked(uint8_t pin)
{
    Serial.println("PrompterPage: onBtnDoubleClicked");
}

void PrompterPage::onEncTurned(int value)
{
    Serial.println("PrompterPage: onEncTurned");
}

void PrompterPage::update()
{
}

void PrompterPage::init()
{
    Serial.println("PrompterPage: init");
    pageID = PAGE_PROMPTER;
    // create screen
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
    // title
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "Prompter");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // icon
    createModeIcon(screen, &prompter_icon, LV_PALETTE_DEEP_ORANGE);
}

void PrompterPage::load()
{
    Serial.println("PrompterPage: load");
}

void PrompterPage::unload()
{
    Serial.println("PrompterPage: unload");
}
