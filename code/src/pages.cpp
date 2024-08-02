#include "pages.h"
#include "TunerPage.h"
#include "MetronomePage.h"
#include "PrompterPage.h"

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

    // initialize all pages
    for (uint8_t i = 0; i < MAX_PAGE; i++)
    {
        if (PageArr[i] != nullptr)
        {
            PageArr[i]->init();
        }
        Serial.println("page init: " + String(i));
    }

    // load the first page
    curPageID = PAGE_TUNER;
    if (PageArr[curPageID] != nullptr)
    {
        PageArr[curPageID]->load();
        lv_screen_load(PageArr[curPageID]->screen);
    }
}

void PageManager_::switchPage(PageID pageID)
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
            lv_screen_load(PageArr[curPageID]->screen);
        }
    }
}

PageManager_ &PageManager = PageManager.getInstance();

void updatePage()
{
    PageManager.getCurPageRef()->update();
}