#pragma once

#include "../os/os.h"

/**
 * Collection of default styles and helper functions to create common GUI-Elements
 */
struct Styles {
    /**
     * Standard background style
     */
    lv_style_t background;
    /**
     * Standard style for default text labels
     */
    lv_style_t textLabel;
    /**
     * Standard style for title labels
     */
    lv_style_t titleLabel;
    /**
     * Standard background style for buttons
     */
    lv_style_t buttonBG;
    /** 
     * Standard style for button lables
     */
    lv_style_t buttonLabel;
    /**
     * Spinner object, for internal use
     */
    lv_obj_t* spinner = nullptr;

    /**
     * Creates a scrollable page with the standard background
     * @param myScr Pointer to the current screen (normally just myScr of the app)
     */
    lv_obj_t* stdPage(lv_obj_t* myScr) {
        lv_obj_t* page = lv_page_create(myScr,NULL);
        lv_obj_set_size(page, 240, 240);
        lv_page_set_scrlbar_mode(page, LV_SCRLBAR_MODE_OFF);
        lv_obj_add_style(page, LV_OBJ_PART_MAIN, &background);
        lv_obj_align(page, NULL, LV_ALIGN_CENTER, 0, 0);
        return page;
    }

    /**
     * Creates a default styled label.
     * @param parent The parent of the label.
     * @param text The text of the label
     */
    lv_obj_t* stdLabel(lv_obj_t* parent, const char* text) {
        lv_obj_t* label = lv_label_create(parent, NULL);
        lv_label_set_text(label, text);
        lv_obj_add_style(label, LV_OBJ_PART_MAIN, &textLabel);
        return label;
    }

    /**
     * Creates a default styled button with a given text and optionally sets the callback.
     * @param parent The parent of the button.
     * @param text The text of the button
     * @param callback The function called, when the button gets an event.
     */
    lv_obj_t* stdButton(lv_obj_t* parent, const char* text, lv_event_cb_t callback=nullptr) {
        lv_obj_t* button = lv_btn_create(parent, NULL);
        lv_obj_add_style(button, LV_OBJ_PART_MAIN, &buttonBG);
        lv_obj_t* label = lv_label_create(button,NULL);
        lv_label_set_text(label, text);
        lv_obj_add_style(label, LV_OBJ_PART_MAIN, &buttonLabel);
        if (callback!=nullptr) {
            lv_obj_set_event_cb(button, callback);
        }
        return button;
    }

    /**
     * Creates a default styled title label.
     * @param parent The parent of the label.
     * @param text The text of the label
     */
    lv_obj_t* stdTitle(lv_obj_t* parent, const char* text) {
        lv_obj_t* title = lv_label_create(parent,NULL);
        lv_obj_add_style(title, LV_OBJ_PART_MAIN, &titleLabel);
        lv_label_set_text(title, text);
        return title;
    }
    /**
     * Shows a spinner on top of a given object. It will be created if necessary.
     * @param scr Typically the myScr screen of the app.
     */
    void showSpinner(lv_obj_t* scr) {
        if (spinner==nullptr) {
            spinner = lv_spinner_create(scr, NULL);
            lv_obj_set_size(spinner, 100, 100);
            lv_obj_align(spinner, NULL, LV_ALIGN_CENTER, 0, 0);
        }
    }
    /** 
     * Hides and destroys the spinner (if not already done).
     */
    void hideSpinner() {
        if (spinner!=nullptr) {
            lv_obj_del(spinner);
            spinner = nullptr;
        }
    }

    /**
     * Initialize all styles, must be called on setup.
     */
    void setup() {
        lv_style_init(&background);
        lv_style_set_bg_color(&background, LV_STATE_DEFAULT,  LV_COLOR_MAKE(0,0,100));
        lv_style_set_bg_grad_color(&background, LV_STATE_DEFAULT,LV_COLOR_MAKE(100,0,0));
        lv_style_set_bg_grad_dir(&background, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
        lv_style_set_border_width(&background, LV_STATE_DEFAULT, 0);

        lv_style_set_radius(&background, LV_OBJ_PART_MAIN, 0);

        lv_style_init(&titleLabel);
        lv_style_set_text_color(&titleLabel, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xff, 0x60, 0xc0));
        lv_style_set_text_font(&titleLabel, LV_STATE_DEFAULT, &lv_font_montserrat_28);
        
        lv_style_init(&buttonLabel);
        lv_style_set_text_color(&buttonLabel, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xb0, 0xb0, 0xff));
        lv_style_set_text_font(&buttonLabel, LV_STATE_DEFAULT, &lv_font_montserrat_22);
        
        lv_style_init(&buttonBG);
        lv_style_set_bg_color(&buttonBG, LV_STATE_DEFAULT,  LV_COLOR_MAKE(0,0,100));
        lv_style_set_bg_grad_color(&buttonBG, LV_STATE_DEFAULT,LV_COLOR_MAKE(100,0,0));
        lv_style_set_bg_grad_dir(&buttonBG, LV_STATE_DEFAULT, LV_GRAD_DIR_HOR);
        lv_style_set_shadow_width(&buttonBG, LV_STATE_DEFAULT, 4);
        lv_style_set_shadow_color(&buttonBG, LV_STATE_DEFAULT, LV_COLOR_MAKE(0,200,100));
        lv_style_set_shadow_ofs_x(&buttonBG, LV_STATE_DEFAULT, 2);
        lv_style_set_shadow_ofs_y(&buttonBG, LV_STATE_DEFAULT, 2);


        lv_style_init(&textLabel);
        lv_style_set_text_color(&textLabel, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xa0, 0xff, 0xa0));
        lv_style_set_text_font(&textLabel, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    }    
};


/**
 * Global Styles object
 */
extern Styles styles;