#include "PrompterPage.h"
#include "hardware.h"

void PrompterPage::onBtnPressed(uint8_t pin)
{
    Serial.println("PrompterPage: onBtnPressed");
}

void PrompterPage::onBtnHold(uint8_t pin)
{
    Serial.println("PrompterPage: onBtnHold");
    if (pin == ENC_BTN)
    {
        PageManager.switchPage(PAGE_METRONOME);
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
}

void PrompterPage::load()
{
    Serial.println("PrompterPage: load");
}

void PrompterPage::unload()
{
    Serial.println("PrompterPage: unload");
}
