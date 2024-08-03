#include "guiUtility.h"

lv_obj_t* createOptionListItem(lv_obj_t *list, const void *icon, const char *text, const char *value)
{
    lv_obj_t *btn = lv_list_add_button(list, icon, text);
    lv_obj_t *valueLabel = lv_label_create(btn);
    lv_label_set_text(valueLabel, value);
    lv_obj_set_style_text_font(btn, &lv_font_montserrat_20, 0);
    // set differnt bg color for focused
    lv_obj_set_style_bg_color(btn, lv_palette_darken(LV_PALETTE_BLUE, 3), LV_STATE_FOCUSED);
    // set value label style
    lv_obj_set_style_radius(valueLabel, 5, LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(valueLabel, lv_color_white(), LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(valueLabel, LV_OPA_100, LV_STATE_CHECKED);
    lv_obj_set_style_text_color(valueLabel, lv_color_black(), LV_STATE_CHECKED);

    return btn;
}

lv_obj_t* getOptionListItemValueLabel(lv_obj_t *listItem)
{
    return lv_obj_get_child(listItem, -1);
}