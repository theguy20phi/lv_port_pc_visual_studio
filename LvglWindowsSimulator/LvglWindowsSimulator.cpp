#include <Windows.h>

#include <LvglWindowsIconResource.h>

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <string>
#include <vector>

struct GUISize {
    const int width;
    const int height;
};

struct GUIPosition {
    const int x;
    const int y;
    const lv_align_t align{ LV_ALIGN_TOP_LEFT };
};

lv_obj_t* createButton(lv_obj_t* scr,
    const std::string& text,
    const GUISize &size,
    const GUIPosition& position,
    const std::vector<lv_style_t*>& idleStyles = {},
    const std::vector<lv_style_t*>& pressedStyles = {}) {
    lv_obj_t* button{ lv_button_create(scr) };
    lv_obj_set_size(button, size.width, size.height);
    for(lv_style_t* style : idleStyles)
        lv_obj_add_style(button, style, 0);
    for (lv_style_t* style : pressedStyles)
        lv_obj_add_style(button, style, LV_STATE_PRESSED);
    lv_obj_align(button, position.align, position.x, position.y);

    lv_obj_t* label{ lv_label_create(button) };
    lv_label_set_text(label, text.c_str());
    lv_obj_center(label);

    return button;
};

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

    static constexpr int bgBorderWidth{ 5 };
    static constexpr int workingWidth{ 480 - bgBorderWidth };
    static constexpr int workingHeight{ 240 - bgBorderWidth };
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
    lv_style_set_border_width(&style_bg, bgBorderWidth);
    lv_style_set_border_color(&style_bg, darkGrey);

    static lv_style_t left_border;
    lv_style_init(&left_border);
    lv_style_set_border_side(&left_border, static_cast<lv_border_side_t>(LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_LEFT));
    lv_style_set_border_color(&left_border, white);
    lv_style_set_border_width(&left_border, 2);
    lv_style_set_radius(&left_border, 5);

    static lv_style_t right_border;
    lv_style_init(&right_border);
    lv_style_set_border_side(&right_border, static_cast<lv_border_side_t>(LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT));
    lv_style_set_border_color(&right_border, white);
    lv_style_set_border_width(&right_border, 2);
    lv_style_set_radius(&right_border, 5);

    static lv_style_t style_title;
    lv_style_init(&style_title);
    lv_style_set_text_color(&style_title, black);
    lv_style_set_text_font(&style_title, &lv_font_montserrat_36);
    lv_style_set_text_align(&style_title, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_letter_space(&style_title, -4);
    lv_style_set_bg_color(&style_title, lightGrey);
    lv_style_set_bg_opa(&style_title, LV_OPA_COVER);

    static lv_style_t style_button_idle;
    lv_style_init(&style_button_idle);
    lv_style_set_text_color(&style_button_idle, black);
    lv_style_set_text_font(&style_button_idle, &lv_font_montserrat_36);
    lv_style_set_text_letter_space(&style_button_idle, -4);
    lv_style_set_bg_color(&style_button_idle, lightGrey);
    lv_style_set_bg_opa(&style_button_idle, LV_OPA_COVER);
    lv_style_set_shadow_opa(&style_button_idle, LV_OPA_TRANSP);

    static lv_style_t style_button_pressed;
    lv_style_init(&style_button_pressed);
    lv_style_set_text_color(&style_button_pressed, white);
    lv_style_set_bg_color(&style_button_pressed, black);

    static lv_style_t style_button_matrix;
    lv_style_init(&style_button_matrix);
    lv_style_set_bg_opa(&style_button_matrix, LV_OPA_TRANSP);
    

    lv_obj_t* scr{ lv_screen_active() };
    lv_obj_add_style(scr, &style_bg, 0);

    lv_obj_t* label{ lv_label_create(scr) };
    lv_label_set_text(label, "HOME");
    lv_obj_set_size(label, 250, 40);
    lv_obj_add_style(label, &right_border, 0);
    lv_obj_add_style(label, &style_title, 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 5, 5);

    createButton(scr,
        LV_SYMBOL_NEW_LINE,
        { 150, 40 },
        { -5, 5, LV_ALIGN_TOP_RIGHT },
        { &left_border, &style_button_idle },
        { &style_button_pressed }
    );

    createButton(scr,
        "ROUTINE",
        { workingWidth - 20, 40 },
        { 0, -140, LV_ALIGN_BOTTOM_MID },
        { &style_button_idle },
        { &style_button_pressed }
    );

    createButton(scr,
        "LOG",
        { workingWidth - 20, 40 },
        { 0, -95, LV_ALIGN_BOTTOM_MID },
        { &style_button_idle },
        { &style_button_pressed }
    );

    createButton(scr,
        "GRAPH",
        { workingWidth - 20, 40 },
        { 0, -50, LV_ALIGN_BOTTOM_MID },
        { &style_button_idle },
        { &style_button_pressed }
    );

    createButton(scr,
        "MAP",
        { workingWidth - 20, 40 },
        { 0, -5, LV_ALIGN_BOTTOM_MID },
        { &style_button_idle },
        { &style_button_pressed }
    );


    //lv_demo_widgets();
    //lv_demo_benchmark();

    while (1)
    {
        uint32_t time_till_next = lv_timer_handler();
        lv_delay_ms(time_till_next);
    }

    return 0;
}
