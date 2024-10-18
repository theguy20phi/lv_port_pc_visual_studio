#include <Windows.h>

#include <LvglWindowsIconResource.h>

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"


int main()
{
    lv_init();

    /*
     * Optional workaround for users who wants UTF-8 console output.
     * If you don't want that behavior can comment them out.
     *
     * Suggested by jinsc123654.
     */
#if LV_TXT_ENC == LV_TXT_ENC_UTF8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    int32_t zoom_level = 100;
    bool allow_dpi_override = false;
    bool simulator_mode = true;
    lv_display_t* display = lv_windows_create_display(
        L"LVGL Windows Simulator Display 1",
        480,
        240,
        zoom_level,
        allow_dpi_override,
        simulator_mode);
    if (!display)
    {
        return -1;
    }

    HWND window_handle = lv_windows_get_display_window_handle(display);
    if (!window_handle)
    {
        return -1;
    }

    HICON icon_handle = LoadIconW(
        GetModuleHandleW(NULL),
        MAKEINTRESOURCE(IDI_LVGL_WINDOWS));
    if (icon_handle)
    {
        SendMessageW(
            window_handle,
            WM_SETICON,
            TRUE,
            (LPARAM)icon_handle);
        SendMessageW(
            window_handle,
            WM_SETICON,
            FALSE,
            (LPARAM)icon_handle);
    }

    lv_indev_t* pointer_indev = lv_windows_acquire_pointer_indev(display);
    if (!pointer_indev)
    {
        return -1;
    }

    lv_indev_t* keypad_indev = lv_windows_acquire_keypad_indev(display);
    if (!keypad_indev)
    {
        return -1;
    }

    lv_indev_t* encoder_indev = lv_windows_acquire_encoder_indev(display);
    if (!encoder_indev)
    {
        return -1;
    }

    const lv_color_t black{ lv_color_hex(0x1f1f1f) };
    const lv_color_t darkGrey{ lv_color_hex(0x303030) };
    const lv_color_t grey{ lv_color_hex(0x454545) };
    const lv_color_t lightGrey{ lv_color_hex(0xbebebe) };
    const lv_color_t white{ lv_color_white()};
    
    

    static lv_style_t style_bg;
    lv_style_init(&style_bg);
    lv_style_set_bg_color(&style_bg, darkGrey);
    lv_style_set_bg_opa(&style_bg, LV_OPA_COVER);
    lv_style_set_border_opa(&style_bg, LV_OPA_COVER);
    lv_style_set_border_width(&style_bg, 10);
    lv_style_set_border_color(&style_bg, grey);


    static lv_style_t style_title;
    lv_style_init(&style_title);
    lv_style_set_text_color(&style_title, white);
    lv_style_set_text_font(&style_title, &lv_font_montserrat_36);
    lv_style_set_text_letter_space(&style_title, -4);
    lv_style_set_bg_color(&style_title, black);
    lv_style_set_bg_opa(&style_title, LV_OPA_COVER);
    /*
    lv_style_set_text_decor(&style_title, LV_TEXT_DECOR_UNDERLINE);
    lv_style_set_radius(&style_title, 10);
    lv_style_set_transform_width(&style_title, 10);
    lv_style_set_transform_height(&style_title, -1);
    lv_style_set_border_color(&style_title, lv_color_lighten(atuGreen, 50));
    lv_style_set_border_width(&style_title, 3);
    lv_style_set_border_opa(&style_title, LV_OPA_COVER);
    lv_style_set_border_side(&style_title, static_cast<lv_border_side_t>(LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT));
    lv_style_set_shadow_opa(&style_title, LV_OPA_50);
    lv_style_set_shadow_color(&style_title, lv_color_darken(atuGreen, 150));
    lv_style_set_shadow_width(&style_title, 15);
    lv_style_set_shadow_spread(&style_title, 5);
    */



    lv_obj_t* scr{ lv_screen_active() };
    lv_obj_add_style(scr, &style_bg, 0);

    lv_obj_t* label{ lv_label_create(scr) };
    lv_label_set_text(label, "HOME");
    lv_obj_add_style(label, &style_title, 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 50, 0);


    //lv_demo_widgets();
    //lv_demo_benchmark();

    while (1)
    {
        uint32_t time_till_next = lv_timer_handler();
        lv_delay_ms(time_till_next);
    }

    return 0;
}
