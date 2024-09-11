#include "guiUtility.h"

lv_obj_t* createOptionListItem(lv_obj_t *list, const void *icon, const char *text, const char *value, lv_palette_t palette)
{
    lv_obj_t *btn = lv_list_add_button(list, icon, text);
    lv_obj_t *valueLabel = lv_label_create(btn);
    lv_label_set_text(valueLabel, value);
    lv_obj_set_style_text_font(btn, &lv_font_montserrat_20, 0);
    // set differnt bg color for focused
    lv_obj_set_style_bg_color(btn, lv_palette_darken(palette, 4), LV_STATE_FOCUSED);
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

lv_obj_t* createArc(lv_obj_t *parent, lv_palette_t palette, const char* unit, int32_t size, int32_t width)
{
    // create arc
    lv_obj_t *arc = lv_arc_create(parent);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 0);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_arc_width(arc, width, 0);
    lv_obj_set_style_size(arc, size, size, 0);
    lv_obj_set_style_pad_all(arc, 10, 0);
   
    // indicator part
    lv_obj_set_style_arc_width(arc, width, LV_PART_INDICATOR);
    // knob part
    lv_obj_set_style_pad_all(arc, width/3, LV_PART_KNOB);
    
    lv_obj_add_flag(arc, LV_OBJ_FLAG_OVERFLOW_VISIBLE);

    // create value
    lv_obj_t *valueText = lv_label_create(arc);
    lv_obj_center(valueText);
    // create unit
    if (unit != NULL)
    {
        lv_obj_t *unitText = lv_label_create(arc);
        lv_label_set_text(unitText, unit);
        lv_obj_align(unitText, LV_ALIGN_BOTTOM_MID, 0, 0);
    }
    // set color
    lv_obj_set_style_arc_color(arc, lv_palette_darken(palette, 4), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_palette_lighten(palette, 4), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(arc, lv_palette_lighten(palette, 4), LV_PART_KNOB);

    return arc;
}

lv_obj_t* getArcValueLabel(lv_obj_t *arc)
{
    return lv_obj_get_child(arc, 0);
}

lv_obj_t* getArcUnitLabel(lv_obj_t *arc)
{
    return lv_obj_get_child(arc, 1);
}

lv_obj_t *createModeIcon(lv_obj_t *parent, const void *imgSrc, lv_palette_t palette)
{
    lv_obj_t *icon = lv_image_create(parent);
    lv_image_set_src(icon, imgSrc);
    lv_image_set_scale(icon, 200);
    lv_obj_align(icon, LV_ALIGN_BOTTOM_LEFT, 5, -5);
    lv_obj_set_style_bg_color(icon, lv_palette_darken(palette, 4), 0);
    lv_obj_set_style_bg_opa(icon, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(icon, 10, 0);
    lv_obj_set_size(icon, 50, 50);
    lv_image_set_inner_align(icon, LV_ALIGN_CENTER);
    return icon;
}
