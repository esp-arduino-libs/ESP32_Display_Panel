// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: Porting

#include "../ui.h"

void ui_ScreenMain_screen_init(void)
{
ui_ScreenMain = lv_obj_create(NULL);
lv_obj_clear_flag( ui_ScreenMain, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_ScreenMain, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_ScreenMain, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Image1ScreenMain = lv_img_create(ui_ScreenMain);
lv_img_set_src(ui_Image1ScreenMain, &ui_img_lexin_1_png);
lv_obj_set_width( ui_Image1ScreenMain, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Image1ScreenMain, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Image1ScreenMain, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image1ScreenMain, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image1ScreenMain, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_Image2ScreenMain = lv_img_create(ui_ScreenMain);
lv_img_set_src(ui_Image2ScreenMain, &ui_img_lexin_2_png);
lv_obj_set_width( ui_Image2ScreenMain, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Image2ScreenMain, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Image2ScreenMain, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Image2ScreenMain, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
lv_obj_clear_flag( ui_Image2ScreenMain, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

lv_obj_add_event_cb(ui_Image1ScreenMain, ui_event_Image1ScreenMain, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Image2ScreenMain, ui_event_Image2ScreenMain, LV_EVENT_ALL, NULL);

}
