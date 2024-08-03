#ifndef GUI_UTILITY_H
#define GUI_UTILITY_H

#include <lvgl.h>

#define createAdjustingText(x) (String(LV_SYMBOL_LEFT " ") + x + String(" " LV_SYMBOL_RIGHT)).c_str()
#define removeAdjustingText(x) (x.substring(4, x.length() - 4)).c_str()

lv_obj_t* createOptionListItem(lv_obj_t *list, const void *icon, const char *text, const char *value);
lv_obj_t* getOptionListItemValueLabel(lv_obj_t *listItem);
#endif // GUI_UTILITY_H