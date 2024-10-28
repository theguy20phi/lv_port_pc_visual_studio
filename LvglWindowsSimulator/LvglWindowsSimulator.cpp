#include <Windows.h>

#include <LvglWindowsIconResource.h>

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>

static lv_style_t styleButtonIdle;
static lv_style_t styleButtonPressed;
static lv_obj_t* homeScreen;
static lv_obj_t* routinesScreen;

struct GUISize {
    const int width;
    const int height;
};

struct GUIPosition {
    const int x;
    const int y;
    const lv_align_t align{ LV_ALIGN_TOP_LEFT };
};

void addStyles(lv_obj_t* obj,
    const std::vector<lv_style_t*>& styles,
    const lv_style_selector_t selector = LV_STATE_DEFAULT) {
    for (lv_style_t* style : styles)
        lv_obj_add_style(obj, style, selector);
}

static void changeScreenCB(lv_event_t* event) {
    void* userData{ lv_event_get_user_data(event) };
    lv_obj_t* scr{ static_cast<lv_obj_t*>(userData) };
    if (scr)
        lv_screen_load(scr);
}

static lv_obj_t* createScreenChangeButton(lv_obj_t* currentScr,
    const std::string& text,
    const GUISize &size,
    const GUIPosition& position,
    lv_obj_t* nextScr = nullptr,
    const std::vector<lv_style_t*>& extraIdleStyles = {},
    const std::vector<lv_style_t*>& extraPressedStyles = {}) {
    // Create the button, size it, and place it.
    lv_obj_t* button{ lv_button_create(currentScr) };
    lv_obj_set_size(button, size.width, size.height);
    lv_obj_align(button, position.align, position.x, position.y);

    // Add and place the text on the button. 
    lv_obj_t* label{ lv_label_create(button) };
    lv_label_set_text(label, text.c_str());
    lv_obj_center(label);

    // Add screen change callback with next screen as a parameter. 
    lv_obj_add_event_cb(button, changeScreenCB, LV_EVENT_CLICKED, static_cast<void*>(nextScr));

    // Add styles. 
    lv_obj_add_style(button, &styleButtonIdle, 0);
    addStyles(button, extraIdleStyles);
    lv_obj_add_style(button, &styleButtonPressed, LV_STATE_PRESSED);
    addStyles(button, extraPressedStyles, LV_STATE_PRESSED);

    return button;
};

static lv_obj_t* createLabel(lv_obj_t* scr,
    const std::string& text,
    const GUISize& size,
    const GUIPosition& position,
    const std::vector<lv_style_t*>& styles = {}) {
    lv_obj_t* label{ lv_label_create(scr) };
    lv_label_set_text(label, text.c_str());
    lv_obj_set_size(label, size.width, size.height);
    addStyles(label, styles);
    lv_obj_align(label, position.align, position.x, position.y);
    return label;
}

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
    static constexpr int defaultHeight{ 40 };
    static constexpr int defaultPadding{ bgBorderWidth };
    static constexpr int fillWidth{ workingWidth - 4 * defaultPadding };
    static constexpr int fillHeight{ workingHeight - defaultHeight - 4 * defaultPadding};
    static constexpr int contentYOffset{ defaultHeight + 2 * defaultPadding };
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

    lv_style_init(&styleButtonIdle);
    lv_style_set_text_color(&styleButtonIdle, black);
    lv_style_set_text_font(&styleButtonIdle, &lv_font_montserrat_36);
    lv_style_set_text_letter_space(&styleButtonIdle, -4);
    lv_style_set_bg_color(&styleButtonIdle, lightGrey);
    lv_style_set_bg_opa(&styleButtonIdle, LV_OPA_COVER);
    lv_style_set_shadow_opa(&styleButtonIdle, LV_OPA_TRANSP);

    lv_style_init(&styleButtonPressed);
    lv_style_set_text_color(&styleButtonPressed, white);
    lv_style_set_bg_color(&styleButtonPressed, black);

    // Create screens. 
    homeScreen = lv_obj_create(nullptr);
    lv_obj_add_style(homeScreen, &style_bg, 0);
    routinesScreen = lv_obj_create(nullptr);
    lv_obj_add_style(routinesScreen, &style_bg, 0);

    // Home screen setup. 
    createLabel(homeScreen, "HOME", { 250, defaultHeight }, { 5, 5 }, { &right_border, &style_title });

    createScreenChangeButton(homeScreen,
        LV_SYMBOL_NEW_LINE,
        { 150, 40 },
        { -5, 5, LV_ALIGN_TOP_RIGHT },
        nullptr,
        { &left_border }
    );

    createScreenChangeButton(homeScreen,
        "ROUTINE",
        { fillWidth, defaultHeight },
        { 0, contentYOffset, LV_ALIGN_TOP_MID },
        routinesScreen
    );

    createScreenChangeButton(homeScreen,
        "LOG",
        { fillWidth, defaultHeight },
        { 0, contentYOffset + defaultHeight + defaultPadding, LV_ALIGN_TOP_MID }
    );

    createScreenChangeButton(homeScreen,
        "GRAPH",
        { fillWidth, defaultHeight },
        { 0, contentYOffset + 2 * (defaultHeight + defaultPadding), LV_ALIGN_TOP_MID }
    );

    createScreenChangeButton(homeScreen,
        "MAP",
        { fillWidth, defaultHeight },
        { 0, contentYOffset + 3 * (defaultHeight + defaultPadding), LV_ALIGN_TOP_MID }
    );

    // Routines screen setup. 
    createLabel(routinesScreen, "ROUTINES", { 250, defaultHeight }, { 5, 5 }, { &right_border, &style_title });

    createScreenChangeButton(routinesScreen,
        LV_SYMBOL_NEW_LINE,
        { 150, 40 },
        { -5, 5, LV_ALIGN_TOP_RIGHT },
        homeScreen,
        { &left_border }
    );

    lv_obj_t* routineSelections{ lv_dropdown_create(routinesScreen) };
    lv_dropdown_set_options(routineSelections,
        "None\n"
        "Simple Left\n"
        "Simple Right\n"
        "WP Left\n"
        "WP Right\n"
        "Defensive Left\n"
        "Defensive Right\n"
        "Offensive Left"
    );
    lv_dropdown_add_option(routineSelections, "Placeholder", LV_DROPDOWN_POS_LAST);
    lv_dropdown_add_option(routineSelections, "Testing", LV_DROPDOWN_POS_LAST);
    lv_obj_align(routineSelections, LV_ALIGN_TOP_MID, 0, contentYOffset);
    lv_obj_set_size(routineSelections, fillWidth, defaultHeight);
    lv_obj_t* routineSelectionsList{ lv_dropdown_get_list(routineSelections) };
    lv_obj_set_style_max_height(routineSelectionsList, fillHeight - defaultHeight, LV_STATE_DEFAULT);
    lv_obj_add_style(routineSelections, &styleButtonIdle, LV_STATE_DEFAULT);
    lv_obj_add_style(routineSelections, &styleButtonPressed, LV_STATE_PRESSED);
    lv_obj_add_style(routineSelectionsList, &styleButtonIdle, LV_STATE_DEFAULT);
    lv_obj_add_style(routineSelectionsList, &styleButtonPressed, LV_STATE_PRESSED);




    // Load starting screen.
    lv_scr_load(homeScreen);


    //lv_demo_widgets();
    //lv_demo_benchmark();

    while (1)
    {
        uint32_t time_till_next = lv_timer_handler();
        lv_delay_ms(time_till_next);
    }

    return 0;
}
