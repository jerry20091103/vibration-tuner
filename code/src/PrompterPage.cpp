#include "PrompterPage.h"
#include "hardware.h"
#include "guiUtility.h"
#include "Prompter.h"

LV_IMG_DECLARE(prompter_icon);

void PrompterPage::onBtnPressed(uint8_t pin)
{
    prompter.start();
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
    PageManager.switchPage(PAGE_PROMPTER_OPTIONS);
}

void PrompterPage::onEncTurned(int value)
{
    if (value > 0)
    {
        // do nothing
    }
    else
    {
        prompter.setCurrentBeat(0);
    }
}

void PrompterPage::update()
{
    // check if beat count is updated
    if (prompter.getCurrentBeat() != currentBeat)
    {
        currentBeat = prompter.getCurrentBeat();
        // update chords
        std::vector<std::string> chords = prompter.getNext8Chord();
        for (int i = 1; i < 8; i++)
        {
            lv_label_set_text(chordLabel[i], chords[i-1].c_str());
        }
        lv_label_set_text(chordLabel[0], prompter.getCurrentChord().c_str());
        // update beat and bar
        String beatBar = String(prompter.getCurrentBeat()) + "/" + String(prompter.getCurrentBar());
        lv_label_set_text(beatLabel, beatBar.c_str());
    }
    
}

void PrompterPage::init()
{
    Serial.println("PrompterPage: init");
    pageID = PAGE_PROMPTER;
    // create screen
    screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);

    // create 8 labels for 1 current chord and 7 next chords
    chordLabel[0] = lv_label_create(screen);
    lv_label_set_text(chordLabel[0], "Cm7");
    lv_obj_set_style_text_font(chordLabel[0], &lv_font_montserrat_24, 0);
    lv_obj_align(chordLabel[0], LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_pad_all(chordLabel[0], 5, 0);
    lv_obj_set_style_border_width(chordLabel[0], 2, 0);
    lv_obj_set_style_border_color(chordLabel[0], lv_palette_darken(LV_PALETTE_DEEP_ORANGE, 4), 0);
    lv_obj_set_style_radius(chordLabel[0], 10, 0);

    for (int i = 1; i < 8; i++)
    {
        chordLabel[i] = lv_label_create(screen);
        lv_label_set_text(chordLabel[i], "C#m7");
        lv_obj_set_style_text_font(chordLabel[i], &lv_font_montserrat_24, 0);
        lv_obj_align(chordLabel[i], LV_ALIGN_TOP_LEFT, 100, 34 * i - 34);
    }
    // show beat / bar count
    beatLabel = lv_label_create(screen);
    lv_label_set_text(beatLabel, "0/0");
    lv_obj_set_style_text_font(beatLabel, &lv_font_montserrat_24, 0);
    lv_obj_align(beatLabel, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    // icon
    createModeIcon(screen, &prompter_icon, LV_PALETTE_DEEP_ORANGE);
}

void PrompterPage::load()
{
    // set encoder to direction mode
    enc0->changePrecision(0, 0);
}

void PrompterPage::unload()
{
    Serial.println("PrompterPage: unload");
}
