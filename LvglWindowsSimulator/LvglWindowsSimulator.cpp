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
    lv_style_set_bg_color(&style_bg, grey);
    lv_style_set_bg_opa(&style_bg, LV_OPA_COVER);
    lv_style_set_border_opa(&style_bg, LV_OPA_COVER);
    lv_style_set_border_width(&style_bg, 10);
    lv_style_set_border_color(&style_bg, darkGrey);


    static lv_style_t style_title;
    lv_style_init(&style_title);
    lv_style_set_text_color(&style_title, black);
    lv_style_set_text_font(&style_title, &lv_font_montserrat_36);
    lv_style_set_text_letter_space(&style_title, -4);
    lv_style_set_bg_color(&style_title, lightGrey);
    lv_style_set_bg_opa(&style_title, LV_OPA_COVER);
    lv_style_set_border_opa(&style_title, LV_OPA_COVER);
    lv_style_set_border_side(&style_title, static_cast<lv_border_side_t>(LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT));
    lv_style_set_border_color(&style_title, white);
    lv_style_set_border_width(&style_title, 2);
    lv_style_set_radius(&style_title, 5);

    static lv_style_t style_return;
    lv_style_init(&style_return);
    lv_style_set_text_color(&style_return, black);
    lv_style_set_text_font(&style_return, &lv_font_montserrat_36);
    lv_style_set_text_letter_space(&style_return, -4);
    lv_style_set_bg_color(&style_return, lightGrey);
    lv_style_set_bg_opa(&style_return, LV_OPA_COVER);



    lv_obj_t* scr{ lv_screen_active() };
    lv_obj_add_style(scr, &style_bg, 0);

    lv_obj_t* label{ lv_label_create(scr) };
    lv_label_set_text(label, "HOME");
    lv_obj_add_style(label, &style_title, 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 5, 5);

    lv_obj_t* return_button{ lv_button_create(scr) };
    lv_obj_set_size(return_button, 150, 40);
    lv_obj_add_style(return_button, &style_return, 0);
    lv_obj_align(return_button, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_t* return_label{ lv_label_create(return_button) };
    lv_obj_add_style(return_label, &style_return, 0);
    lv_label_set_text(return_label, LV_SYMBOL_NEW_LINE);
    lv_obj_center(return_label);


    //lv_demo_widgets();
    //lv_demo_benchmark();

    while (1)
    {
        uint32_t time_till_next = lv_timer_handler();
        lv_delay_ms(time_till_next);
    }

    return 0;
}
