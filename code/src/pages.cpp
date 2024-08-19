#include "pages.h"
#include "TunerPage.h"
#include "MetronomePage.h"
#include "PrompterPage.h"
#include "MetronomeOptionsPage.h"

PageManager_ &PageManager_::getInstance()
{
    static PageManager_ instance;
    return instance;
}

Pages *PageManager_::getCurPageRef()
{
    return PageArr[curPageID];
}

void PageManager_::init()
{
    // construct all pages
    PageArr[PAGE_TUNER] = new TunerPage();
    PageArr[PAGE_METRONOME] = new MetronomePage();
    PageArr[PAGE_PROMPTER] = new PrompterPage();
    PageArr[PAGE_METRONOME_OPTIONS] = new MetronomeOptionsPage();

    // initialize all pages
    for (uint8_t i = 0; i < MAX_PAGE; i++)
    {
        if (PageArr[i] != nullptr)
        {
            PageArr[i]->init();
        }
        Serial.println("page init: " + String(i));
    }
}

void PageManager_::switchPage(PageID pageID, bool useAnim)
{
    if (pageID < MAX_PAGE)
    {
        if (PageArr[curPageID] != nullptr)
        {
            PageArr[curPageID]->unload();
        }
        curPageID = pageID;
        if (PageArr[curPageID] != nullptr)
        {
            PageArr[curPageID]->load();
            if (useAnim)
                lv_screen_load_anim(PageArr[curPageID]->screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 300, 0, false);
            else
                lv_screen_load(PageArr[curPageID]->screen);
        }
    }
}

void PageManager_::loadAll()
{
    for (uint8_t i = 0; i < MAX_PAGE; i++)
    {
        if (PageArr[i] != nullptr)
        {
            curPageID = (PageID)i;
            PageArr[i]->load();
            PageArr[i]->unload();
            Serial.println("page loadAll(): " + String(i));
            Serial.flush();
        }
    }
}

void PageManager_::loadFistPage(PageID pageID)
{
    // load the first page
    curPageID = pageID;
    if (PageArr[curPageID] != nullptr)
    {
        PageArr[curPageID]->load();
        lv_screen_load(PageArr[curPageID]->screen);
    }
}

PageManager_ &PageManager = PageManager.getInstance();

void updatePage()
{
    PageManager.getCurPageRef()->update();
}