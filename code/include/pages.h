#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>
#include <lvgl.h>

enum PageID {
    PAGE_TUNER,
    PAGE_METRONOME,
    PAGE_PROMPTER,
    PAGE_METRONOME_OPTIONS,
    PAGE_PROMPTER_OPTIONS,
    MAX_PAGE
};

// *Pages class
// A virtual class for other pages to inherit from
class Pages
{
public:
    // This function is called when a button is pressed
    virtual void onBtnPressed(uint8_t pin) = 0;
    // This function is called when a button is holded
    virtual void onBtnHold(uint8_t pin) = 0;
    // This function is called when a button is double clicked
    virtual void onBtnDoubleClicked(uint8_t pin) = 0;
    // This function is called when the encoder is turned
    virtual void onEncTurned(int value) = 0;
    
    // this is called regularly to update elements on the screen
    virtual void update() = 0;
    // This is called when the page is first created
    virtual void init() = 0;
    // This is called when the page is switched to
    virtual void load() = 0;
    // This is called when the page is switched away from
    virtual void unload() = 0;

    uint8_t pageID; 
    lv_obj_t *screen;
};

// *PageManager class
// A Singleton class to store and manage all pages
class PageManager_
{
public:
    static PageManager_ &getInstance();
    // Array of pointers to all pages
    Pages *PageArr[MAX_PAGE] = {nullptr};
    
    // Get the current Page instance on screen.
    Pages* getCurPageRef();
    // Initialize all pages at program start
    void init();
    // Switch to a page
    void switchPage(PageID pageID, bool useAnim = false);
    // load all pages once to set user data
    void loadAll();
    void loadFistPage(PageID pageID);
    
private:
    // Private constructor to prevent instantiation
    PageManager_() {}
    PageID curPageID;
};

extern PageManager_ &PageManager;

void updatePage();

#endif // PAGE_H