#include "PrompterPage.h"
#include "hardware.h"
#include "guiUtility.h"
#include "Prompter.h"

LV_IMG_DECLARE(prompter_icon);

void PrompterPage::onBtnPressed(uint8_t pin)
{
    prompter.togglePrompter();
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
    currentBeat += value;
    currentBeat = constrain(currentBeat, 0, prompter.getScoreLength());
    prompter.setCurrentBeat(currentBeat);
}

void PrompterPage::update()
{
    // check if beat count is updated
    if (prompter.getCurrentBeat() != lastBeat)
    {
        lastBeat = prompter.getCurrentBeat();
        currentBeat = lastBeat;
        // update chords
        std::vector<std::string> chords = prompter.getCurrent8Chord();
        for (int i = 0; i < 8; i++)
        {
            lv_label_set_text(chordLabel[i], chords[i].c_str());
        }
        // update beat and bar
        String beatBar = String(prompter.getCurrentBeat() % 4 + 1) + "/" + String(prompter.getCurrentBar() + 1);
        lv_label_set_text(getArcValueLabel(positionArc), beatBar.c_str());
        lv_arc_set_value(positionArc, prompter.getCurrentBeat());
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
    lv_obj_set_width(chordLabel[0], 125);

    for (int i = 1; i < 8; i++)
    {
        chordLabel[i] = lv_label_create(screen);
        lv_label_set_text(chordLabel[i], "C#m7");
        lv_obj_set_style_text_font(chordLabel[i], &lv_font_montserrat_24, 0);
        lv_obj_align(chordLabel[i], LV_ALIGN_TOP_LEFT, 135, 34 * i - 34);
    }
    // position arc
    positionArc = createArc(screen, LV_PALETTE_DEEP_ORANGE, "Bar", 120, 10);
    lv_obj_align(positionArc, LV_ALIGN_TOP_LEFT, 0, 50);
    lv_obj_t *valueLabel = getArcValueLabel(positionArc);
    lv_obj_set_style_text_font(valueLabel, &lv_font_montserrat_24, 0);
    lv_obj_t *unitLabel = getArcUnitLabel(positionArc);
    lv_label_set_text(unitLabel, "Beat/Bar");
    lv_arc_set_range(positionArc, 0, prompter.getScoreLength());
    lv_arc_set_value(positionArc, prompter.getCurrentBeat());

    // bpm label
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "BPM");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, 0, -80);
    lv_obj_set_style_text_color(label, lv_palette_lighten(LV_PALETTE_DEEP_ORANGE, 1), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    // bpm value
    bpmLabel = lv_label_create(screen);
    lv_obj_set_style_text_font(bpmLabel, &lv_font_montserrat_20, 0);
    lv_obj_align(bpmLabel, LV_ALIGN_BOTTOM_RIGHT, 0, -55);
    lv_obj_set_style_text_align(bpmLabel, LV_TEXT_ALIGN_CENTER, 0);
    // speed label
    label = lv_label_create(screen);
    lv_label_set_text(label, "Speed");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, 0, -30);
    lv_obj_set_style_text_color(label, lv_palette_lighten(LV_PALETTE_DEEP_ORANGE, 1), 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    // speed value
    speedLabel = lv_label_create(screen);
    lv_obj_set_style_text_font(speedLabel, &lv_font_montserrat_20, 0);
    lv_obj_align(speedLabel, LV_ALIGN_BOTTOM_RIGHT, 0, -5);
    lv_obj_set_style_text_align(speedLabel, LV_TEXT_ALIGN_CENTER, 0);

    // icon
    createModeIcon(screen, &prompter_icon, LV_PALETTE_DEEP_ORANGE);
}

void PrompterPage::load()
{
    // set encoder to direction mode
    enc0->changePrecision(0, 0);
    // set prompter user data
    prompter.setCurrentBeat(currentBeat);
    // update bpm and speed label
    lv_label_set_text_fmt(bpmLabel, "%d", prompter.getBPM());
    String speed = "x" + String(prompter.getSpeed(), 1); // somehow %.1f not working with lv_label_set_text_fmt
    lv_label_set_text(speedLabel, speed.c_str());
}

void PrompterPage::unload()
{
    Serial.println("PrompterPage: unload");
}
