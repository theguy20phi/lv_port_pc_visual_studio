#include <Windows.h>

#include <LvglWindowsIconResource.h>

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <string>
#include <vector>
#include <iostream>
class GUI {
public:
    static const int screenWidth;
    static const int screenHeight;

    static void startLoading() {
        // Initialization. 
        createScreens();
        initializeStyles();

        // Screen setup.
        loadingScreenSetup();
        homeScreenSetup();
        mainMenuScreenSetup();
        routinesScreenSetup();
        loggerScreenSetup();
        graphScreenSetup();
        mapScreenSetup();

        // Go to the loading screen until given signal robot is good to go. 
        lv_scr_load(loadingScreen);
    }

    static void finishLoading() {
        lv_scr_load(homeScreen);
    }

private:
    // Commonly used dimensions/lengths.
    static const int bgBorderWidth;
    static const int workingWidth;
    static const int workingHeight;
    static const int defaultHeight;
    static const int defaultPadding;
    static const int fillWidth;
    static const int fillHeight;
    static const int contentYOffset;

    // Commonly used colors.
    static const lv_color_t black;
    static const lv_color_t darkGrey;
    static const lv_color_t grey;
    static const lv_color_t lightGrey;
    static const lv_color_t red;
    static const lv_color_t green;
    static const lv_color_t blue;
    static const lv_color_t white;

    // Commonly used styles.
    static lv_style_t styleButtonIdle;
    static lv_style_t styleButtonPressed;
    static lv_style_t styleBG;
    static lv_style_t leftBorder;
    static lv_style_t rightBorder;
    static lv_style_t styleTitle;
    static lv_style_t styleDropDownIdle;
    static lv_style_t styleDropDownPressed;
    static lv_style_t styleChart;

    // Screens.
    static lv_obj_t* loadingScreen;
    static lv_obj_t* mainMenuScreen;
    static lv_obj_t* homeScreen;
    static lv_obj_t* routinesScreen;
    static lv_obj_t* logScreen;
    static lv_obj_t* graphScreen;
    static lv_obj_t* mapScreen;
    static lv_obj_t* logSwitch;
    static lv_obj_t* logTextArea;


    struct GUISize {
        const int width;
        const int height;
    };

    struct GUIPosition {
        const int x;
        const int y;
        const lv_align_t align{ LV_ALIGN_TOP_LEFT };
    };

    static void addStyles(lv_obj_t* obj,
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
        const GUISize& size,
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

    static void initializeStyles() {

        // Background style.
        lv_style_init(&styleBG);
        lv_style_set_bg_color(&styleBG, grey);
        lv_style_set_bg_opa(&styleBG, LV_OPA_COVER);
        lv_style_set_border_opa(&styleBG, LV_OPA_COVER);
        lv_style_set_border_width(&styleBG, bgBorderWidth);
        lv_style_set_border_color(&styleBG, darkGrey);

        // Left border style.
        lv_style_init(&leftBorder);
        lv_style_set_border_side(&leftBorder, static_cast<lv_border_side_t>(LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_LEFT));
        lv_style_set_border_color(&leftBorder, white);
        lv_style_set_border_width(&leftBorder, 2);
        lv_style_set_radius(&leftBorder, 5);

        // Right border style.
        lv_style_init(&rightBorder);
        lv_style_set_border_side(&rightBorder, static_cast<lv_border_side_t>(LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT));
        lv_style_set_border_color(&rightBorder, white);
        lv_style_set_border_width(&rightBorder, 2);
        lv_style_set_radius(&rightBorder, 5);

        // Title label style.
        lv_style_init(&styleTitle);
        lv_style_set_text_color(&styleTitle, black);
        lv_style_set_text_font(&styleTitle, &lv_font_montserrat_36);
        lv_style_set_text_align(&styleTitle, LV_TEXT_ALIGN_CENTER);
        lv_style_set_text_letter_space(&styleTitle, -4);
        lv_style_set_bg_color(&styleTitle, lightGrey);
        lv_style_set_bg_opa(&styleTitle, LV_OPA_COVER);

        // Idle button style.
        lv_style_init(&styleButtonIdle);
        lv_style_set_text_color(&styleButtonIdle, black);
        lv_style_set_text_font(&styleButtonIdle, &lv_font_montserrat_36);
        lv_style_set_text_letter_space(&styleButtonIdle, -4);
        lv_style_set_bg_color(&styleButtonIdle, lightGrey);
        lv_style_set_bg_opa(&styleButtonIdle, LV_OPA_COVER);
        lv_style_set_shadow_opa(&styleButtonIdle, LV_OPA_TRANSP);

        // Pressed button style.
        lv_style_init(&styleButtonPressed);
        lv_style_set_text_color(&styleButtonPressed, white);
        lv_style_set_bg_color(&styleButtonPressed, black);

        // Idle drop down style
        lv_style_init(&styleDropDownIdle);
        lv_style_set_text_color(&styleDropDownIdle, black);
        lv_style_set_text_font(&styleDropDownIdle, &lv_font_montserrat_20);
        lv_style_set_text_align(&styleDropDownIdle, LV_TEXT_ALIGN_CENTER);
        lv_style_set_bg_color(&styleDropDownIdle, lightGrey);
        lv_style_set_bg_opa(&styleDropDownIdle, LV_OPA_COVER);

        // Pressed drop down style.
        lv_style_init(&styleDropDownPressed);
        lv_style_set_text_color(&styleDropDownPressed, white);
        lv_style_set_bg_color(&styleDropDownPressed, black);

        // Chart style.
        lv_style_init(&styleChart);
        lv_style_set_bg_color(&styleChart, black);
    }

    static void createScreens() {
        loadingScreen = lv_obj_create(nullptr);
        lv_obj_set_style_bg_color(loadingScreen, black, LV_PART_MAIN);
        mainMenuScreen = lv_obj_create(nullptr);
        lv_obj_add_style(mainMenuScreen, &styleBG, 0);
        homeScreen = lv_obj_create(nullptr);
        lv_obj_set_style_bg_color(homeScreen, black, LV_PART_MAIN);
        lv_obj_add_event_cb(homeScreen, changeScreenCB, LV_EVENT_CLICKED, static_cast<void*>(mainMenuScreen));
        routinesScreen = lv_obj_create(nullptr);
        lv_obj_add_style(routinesScreen, &styleBG, 0);
        logScreen = lv_obj_create(nullptr);
        lv_obj_add_style(logScreen, &styleBG, 0);
        graphScreen = lv_obj_create(nullptr);
        lv_obj_add_style(graphScreen, &styleBG, 0);
        mapScreen = lv_obj_create(nullptr);
        lv_obj_add_style(mapScreen, &styleBG, 0);
    }

    static void loadingScreenSetup() {
        lv_obj_t* loadingLabel{ lv_label_create(loadingScreen) };
        lv_label_set_text(loadingLabel, "LOADING!   DO NOT TOUCH!");
        lv_obj_center(loadingLabel);
        lv_obj_set_width(loadingLabel, screenWidth);
        lv_label_set_long_mode(loadingLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_style_text_color(loadingLabel, white, LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(loadingLabel, &lv_font_montserrat_48, LV_STATE_DEFAULT);
    }

    static void homeScreenSetup() {
        createLabel(homeScreen, "HOME", { 250, defaultHeight }, { defaultPadding, defaultPadding }, { &rightBorder, &styleTitle });
        lv_obj_t* atumLogo = lv_image_create(homeScreen);
        lv_img_set_src(atumLogo, "C:/Users/brade/Desktop/atum.bin");
        lv_obj_set_style_opa(atumLogo, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_center(atumLogo);
        lv_obj_set_size(atumLogo, screenWidth, screenHeight);

        lv_obj_t* clickAnywhereLabel{ lv_label_create(homeScreen) };
        lv_label_set_text(clickAnywhereLabel, "Click anywhere to continue.");
        lv_obj_center(clickAnywhereLabel);
        lv_obj_align(clickAnywhereLabel, LV_ALIGN_BOTTOM_MID, 0, -2 * defaultPadding);
        lv_obj_set_style_text_color(clickAnywhereLabel, white, LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(clickAnywhereLabel, &lv_font_montserrat_12, LV_STATE_DEFAULT);
    }

    static void mainMenuScreenSetup() {
        createLabel(mainMenuScreen, "MAIN MENU", { 250, defaultHeight }, { defaultPadding, defaultPadding }, { &rightBorder, &styleTitle });

        createScreenChangeButton(mainMenuScreen,
            LV_SYMBOL_NEW_LINE,
            { 150, defaultHeight },
            { -defaultPadding, defaultPadding, LV_ALIGN_TOP_RIGHT },
            homeScreen,
            { &leftBorder }
        );

        createScreenChangeButton(mainMenuScreen,
            "ROUTINE",
            { fillWidth, defaultHeight },
            { 0, contentYOffset, LV_ALIGN_TOP_MID },
            routinesScreen
        );

        createScreenChangeButton(mainMenuScreen,
            "LOG",
            { fillWidth, defaultHeight },
            { 0, contentYOffset + defaultHeight + defaultPadding, LV_ALIGN_TOP_MID },
            logScreen
        );

        createScreenChangeButton(mainMenuScreen,
            "GRAPH",
            { fillWidth, defaultHeight },
            { 0, contentYOffset + 2 * (defaultHeight + defaultPadding), LV_ALIGN_TOP_MID },
            graphScreen
        );

        createScreenChangeButton(mainMenuScreen,
            "MAP",
            { fillWidth, defaultHeight },
            { 0, contentYOffset + 3 * (defaultHeight + defaultPadding), LV_ALIGN_TOP_MID },
            mapScreen
        );
    }

    static void routinesScreenSetup() {
        createLabel(routinesScreen, "ROUTINES", { 250, defaultHeight }, { defaultPadding, defaultPadding }, { &rightBorder, &styleTitle });

        createScreenChangeButton(routinesScreen,
            LV_SYMBOL_NEW_LINE,
            { 150, defaultHeight },
            { -defaultPadding, defaultPadding, LV_ALIGN_TOP_RIGHT },
            mainMenuScreen,
            { &leftBorder }
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
        lv_obj_add_style(routineSelections, &styleDropDownIdle, LV_STATE_DEFAULT);
        lv_obj_add_style(routineSelections, &styleDropDownPressed, LV_STATE_PRESSED);
        lv_obj_add_style(routineSelectionsList, &styleDropDownIdle, LV_STATE_DEFAULT);
        lv_obj_add_style(routineSelectionsList, &styleDropDownPressed, LV_PART_SELECTED | LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(routineSelectionsList, darkGrey, LV_PART_SELECTED | LV_STATE_CHECKED);

        lv_obj_t* colorSwitch{ lv_switch_create(routinesScreen) };
        lv_obj_set_size(colorSwitch, fillWidth, 3 * defaultHeight);
        lv_obj_align(colorSwitch, LV_ALIGN_BOTTOM_MID, 0, -defaultPadding);
        lv_obj_set_style_bg_color(colorSwitch, red, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(colorSwitch, white, LV_PART_KNOB);
        lv_obj_set_style_bg_color(colorSwitch, blue, LV_PART_INDICATOR | LV_STATE_CHECKED);
    }

    static void loggerScreenSetup() {
        createLabel(logScreen, "LOG", { 250, defaultHeight }, { defaultPadding, defaultPadding }, { &rightBorder, &styleTitle });

        createScreenChangeButton(logScreen,
            LV_SYMBOL_NEW_LINE,
            { 150, defaultHeight },
            { -defaultPadding, defaultPadding, LV_ALIGN_TOP_RIGHT },
            mainMenuScreen,
            { &leftBorder }
        );

        lv_obj_t* logEnabledLabel{ lv_label_create(logScreen) };
        lv_label_set_text(logEnabledLabel, "Logging enabled:");
        lv_obj_center(logEnabledLabel);
        lv_obj_align(logEnabledLabel, LV_ALIGN_BOTTOM_RIGHT, -defaultPadding - 125, -3 * defaultPadding);
        lv_obj_set_style_text_color(logEnabledLabel, white, LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(logEnabledLabel, &lv_font_montserrat_18, LV_STATE_DEFAULT);

        logSwitch = lv_switch_create(logScreen);
        lv_obj_set_size(logSwitch, 100, defaultHeight);
        lv_obj_add_state(logSwitch, LV_STATE_CHECKED);
        lv_obj_align(logSwitch, LV_ALIGN_BOTTOM_RIGHT, -defaultPadding, -defaultPadding);
        lv_obj_set_style_bg_color(logSwitch, black, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(logSwitch, white, LV_PART_KNOB);
        lv_obj_set_style_bg_color(logSwitch, white, LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_set_style_bg_color(logSwitch, black, LV_PART_KNOB | LV_STATE_CHECKED);


        logTextArea = lv_textarea_create(logScreen);
        lv_obj_align(logTextArea, LV_ALIGN_TOP_MID, 0, contentYOffset);
        lv_obj_set_size(logTextArea, fillWidth, fillHeight - defaultHeight - defaultPadding);
        lv_obj_set_style_bg_color(logTextArea, lightGrey, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(logTextArea, LV_OPA_COVER, LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(logTextArea, black, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(logTextArea, white, LV_STATE_DEFAULT | LV_PART_SCROLLBAR);
        lv_textarea_set_max_length(logTextArea, 1000);
    }

    static void graphScreenSetup() {
        createLabel(graphScreen, "GRAPH", { 250, defaultHeight }, { defaultPadding, defaultPadding }, { &rightBorder, &styleTitle });

        createScreenChangeButton(graphScreen,
            LV_SYMBOL_NEW_LINE,
            { 150, defaultHeight },
            { -defaultPadding, defaultPadding, LV_ALIGN_TOP_RIGHT },
            mainMenuScreen,
            { &leftBorder }
        );

        lv_obj_t* graphChart{ lv_chart_create(graphScreen) };
        lv_chart_set_type(graphChart, LV_CHART_TYPE_LINE);
        lv_obj_set_size(graphChart, fillWidth, fillHeight);
        lv_obj_align(graphChart, LV_ALIGN_TOP_MID, 0, contentYOffset);
        lv_chart_set_range(graphChart, LV_CHART_AXIS_PRIMARY_Y, -10000, 10000);
        lv_chart_set_div_line_count(graphChart, 9, 18);
        lv_obj_add_style(graphChart, &styleChart, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_chart_series_t* redGraphSeries{ lv_chart_add_series(graphChart, red, LV_CHART_AXIS_PRIMARY_Y) };
        lv_chart_set_next_value(graphChart, redGraphSeries, 2000);
        lv_chart_set_next_value(graphChart, redGraphSeries, 4000);
        lv_chart_set_next_value(graphChart, redGraphSeries, 6000);
        lv_chart_set_next_value(graphChart, redGraphSeries, 8000);
        lv_chart_set_next_value(graphChart, redGraphSeries, 10000);
    }

    static void mapScreenSetup() {
        lv_obj_t* mapLabel{ createLabel(mapScreen, "MAP", { 150, defaultHeight }, { -6 * defaultPadding, -70, LV_ALIGN_BOTTOM_RIGHT }, { &leftBorder, &styleTitle }) };
        lv_obj_set_style_border_side(mapLabel, static_cast<lv_border_side_t>(LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_LEFT), LV_STATE_DEFAULT);

        createScreenChangeButton(mapScreen,
            LV_SYMBOL_NEW_LINE,
            { 150, defaultHeight },
            { -6 * defaultPadding, 70, LV_ALIGN_TOP_RIGHT },
            mainMenuScreen,
            { &leftBorder }
        );

        lv_obj_t* mapChart{ lv_chart_create(mapScreen) };
        lv_chart_set_type(mapChart, LV_CHART_TYPE_SCATTER);
        static const int mapChartSize{ screenHeight - 4 * defaultPadding };
        lv_obj_set_size(mapChart, mapChartSize, mapChartSize);
        lv_obj_align(mapChart, LV_ALIGN_TOP_LEFT, 6 * defaultPadding, defaultPadding);
        lv_chart_set_range(mapChart, LV_CHART_AXIS_PRIMARY_X, -12000, 12000);
        lv_chart_set_range(mapChart, LV_CHART_AXIS_PRIMARY_Y, -12000, 12000);
        lv_chart_set_div_line_count(mapChart, 9, 9);
        lv_obj_set_style_line_width(mapChart, 0, LV_PART_ITEMS);
        lv_obj_add_style(mapChart, &styleChart, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_chart_series_t* redMapSeries{ lv_chart_add_series(mapChart, red, LV_CHART_AXIS_PRIMARY_Y) };
        lv_chart_set_next_value2(mapChart, redMapSeries, 3000, 3000);
        lv_chart_set_next_value2(mapChart, redMapSeries, 6000, 6000);
        lv_chart_set_next_value2(mapChart, redMapSeries, 9000, 9000);
        lv_chart_set_next_value2(mapChart, redMapSeries, 12000, 12000);


        lv_chart_series_t* greenMapSeries{ lv_chart_add_series(mapChart, green, LV_CHART_AXIS_PRIMARY_Y) };
        lv_chart_set_next_value2(mapChart, greenMapSeries, -3000, 3000);
        lv_chart_set_next_value2(mapChart, greenMapSeries, -6000, 6000);
        lv_chart_set_next_value2(mapChart, greenMapSeries, -9000, 9000);
        lv_chart_set_next_value2(mapChart, greenMapSeries, -12000, 12000);
    }
};

// Commonly used dimensions/lengths.
const int GUI::screenWidth{ 480 };
const int GUI::screenHeight{ 240 };
const int GUI::bgBorderWidth{ 5 };
const int GUI::workingWidth{ screenWidth - bgBorderWidth };
const int GUI::workingHeight{ screenHeight - bgBorderWidth };
const int GUI::defaultHeight{ 40 };
const int GUI::defaultPadding{ bgBorderWidth };
const int GUI::fillWidth{ workingWidth - 4 * defaultPadding };
const int GUI::fillHeight{ workingHeight - defaultHeight - 4 * defaultPadding };
const int GUI::contentYOffset{ defaultHeight + 2 * defaultPadding };

// Commonly used colors.
const lv_color_t GUI::black{ lv_color_hex(0x1f1f1f) };
const lv_color_t GUI::darkGrey{ lv_color_hex(0x303030) };
const lv_color_t GUI::grey{ lv_color_hex(0x454545) };
const lv_color_t GUI::lightGrey{ lv_color_hex(0xbebebe) };
const lv_color_t GUI::red{ lv_color_hex(0xFF0000) };
const lv_color_t GUI::green{ lv_color_hex(0x00FF00) };
const lv_color_t GUI::blue{ lv_color_hex(0x0000FF) };
const lv_color_t GUI::white{ lv_color_white() };


// Commonly used styles.
lv_style_t GUI::styleButtonIdle;
lv_style_t GUI::styleButtonPressed;
lv_style_t GUI::styleBG;
lv_style_t GUI::leftBorder;
lv_style_t GUI::rightBorder;
lv_style_t GUI::styleTitle;
lv_style_t GUI::styleDropDownIdle;
lv_style_t GUI::styleDropDownPressed;
lv_style_t GUI::styleChart;

// Screens.
lv_obj_t* GUI::loadingScreen;
lv_obj_t* GUI::mainMenuScreen;
lv_obj_t* GUI::homeScreen;
lv_obj_t* GUI::routinesScreen;
lv_obj_t* GUI::logScreen;
lv_obj_t* GUI::graphScreen;
lv_obj_t* GUI::mapScreen;
lv_obj_t* GUI::logSwitch;
lv_obj_t* GUI::logTextArea;

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
        GUI::screenWidth,
        GUI::screenHeight,
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

    // Start GUI.
    GUI::startLoading();

    //lv_demo_widgets();
    //lv_demo_benchmark();

    int loadingTime{ 4000 };
    int elapsedTime{ 0 };
    bool finishedLoading{ false };
    while (1)
    {
        uint32_t time_till_next = lv_timer_handler();
        elapsedTime += time_till_next;
        if (!finishedLoading && elapsedTime >= loadingTime) {
            GUI::finishLoading();
            finishedLoading = true;
        }
        lv_delay_ms(time_till_next);
    }

    return 0;
}
