/**
 * @author Ivo Blöchliger
 */

#pragma once

#include "../../os/os.h"
#include "fonts/ubuntu_r.h"
#include "fonts/ubuntumono.h"

LV_IMG_DECLARE(stdbackground);


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

    lv_style_t msgBox;
    /**
     * Spinner object, for internal use only
     */
    lv_obj_t* spinner = nullptr;
    /**
     * Spinner label, for internal use only
     */
    lv_obj_t* spinnerLabel = nullptr;

    lv_style_t appTileStyle;

    /**
     * Creates a scrollable page with the standard background
     * @param myScr Pointer to the current screen (normally just myScr of the app)
     * @param useBackgroundImage (default true), if the standard background should be used.
     */
    lv_obj_t* stdPage(lv_obj_t* myScr, bool useBackgroundImage=true) {
        lv_obj_t* page = useBackgroundImage ?  
                lv_page_create(stdBgImage(myScr), NULL) : 
                lv_page_create(myScr, NULL);
        lv_obj_set_size(page, 240, 240);
        lv_page_set_scrlbar_mode(page, LV_SCRLBAR_MODE_OFF);
        lv_obj_add_style(page, LV_OBJ_PART_MAIN, &background);
        lv_obj_align(page, NULL, LV_ALIGN_CENTER, 0, 0);
        return page;
    }

    lv_obj_t* stdBgImage(lv_obj_t* myScr) {
        lv_obj_t* bg = lv_img_create(myScr, NULL);  /*Create an image object*/
        lv_img_set_src(bg, (void *) &stdbackground);
        lv_obj_align(bg, NULL, LV_ALIGN_CENTER, 0, 0);
        return bg;
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
     * @param user_data Userdata stored in the object (typically the this-pointer of the calling instance)
     */
    lv_obj_t* stdButton(lv_obj_t* parent, const char* text, lv_event_cb_t callback=nullptr, void* user_data=nullptr) {
        lv_obj_t* button = lv_btn_create(parent, NULL);
        lv_obj_add_style(button, LV_OBJ_PART_MAIN, &buttonBG);
        lv_obj_t* label = lv_label_create(button,NULL);
        lv_label_set_text(label, text);
        lv_obj_add_style(label, LV_OBJ_PART_MAIN, &buttonLabel);
        if (user_data!=nullptr) {
            lv_obj_set_user_data(button, user_data);
        }
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
     * @param txt Optional Text to be displayed on top of the spinner
     */
    void showSpinner(lv_obj_t* scr, const char* text = nullptr) {
        if (spinner==nullptr) {
            spinner = lv_spinner_create(scr, NULL);
            lv_obj_set_size(spinner, 100, 100);
            lv_obj_align(spinner, NULL, LV_ALIGN_CENTER, 0, 0);
            if (text!=nullptr) {
                spinnerLabel = stdLabel(scr, text);
                lv_obj_align(spinnerLabel, NULL, LV_ALIGN_CENTER, 0, 0);
            } else {
                spinnerLabel = nullptr;
            }
        }
    }
    /** 
     * Hides and destroys the spinner (if not already done).
     */
    void hideSpinner() {
        if (spinner!=nullptr) {
            if (spinnerLabel!=nullptr) {
                lv_obj_del(spinnerLabel);
                spinnerLabel = nullptr;
            }
            lv_obj_del(spinner);
            spinner = nullptr;
        }
    }


    struct MessageBox {
        lv_style_t bgStyle;
        lv_obj_t* bg;
        lv_obj_t* mbox=nullptr;
        std::function<void(uint16_t)> callback;

        void show(char* text, bool autoHide=true) {
            if (mbox==nullptr) {
                //Serial.println("Styles::MessageBox::show(char *text, bool autoHide) start");
                // From https://docs.lvgl.io/latest/en/html/widgets/msgbox.html
                /* Create a base object for the modal background */
                bg = lv_obj_create(lv_scr_act(), NULL);
                lv_obj_reset_style_list(bg, LV_OBJ_PART_MAIN);
                lv_obj_add_style(bg, LV_OBJ_PART_MAIN, &bgStyle);
                //lv_obj_set_style_local_bg_opa(bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
                lv_obj_set_pos(bg, 0, 0);
                lv_obj_set_size(bg, LV_HOR_RES, LV_VER_RES);

                mbox = lv_msgbox_create(bg, NULL);
                lv_msgbox_set_text(mbox, text);
                lv_obj_set_user_data(mbox, this);
                lv_obj_set_event_cb(mbox, mbox_event_cb);
                if (autoHide) {
                    lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0);
                    lv_msgbox_start_auto_close(mbox, 2000);
                }
                //Serial.println("Styles::MessageBox::show(char *text, bool autoHide) end");
            }
        }

        void show(char* text, const char** buttons, std::function<void (uint16_t )> cb) {
            if (mbox==nullptr) {
                //Serial.println("void Styles::MessageBox::show(char *text, char **buttons, std::function<void (uint16_t)> &cb) start");
                callback = cb;
                show(text, false);
                lv_msgbox_add_btns(mbox, buttons);
                //Serial.println("added buttons");
                lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0);
                

                /* Fade the background in with an animation */
                //Serial.println("Starting animation...");
                static lv_anim_t a;
                lv_anim_init(&a);
                lv_anim_set_var(&a, bg);
                lv_anim_set_time(&a, 500);
                lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_50);
                lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)opa_anim);
                lv_anim_start(&a);
                //Serial.println("void Styles::MessageBox::show(char *text, char **buttons, std::function<void (uint16_t)> &cb) end");
            }
        }

        static void opa_anim(lv_obj_t * bg, lv_anim_value_t v) {
            lv_obj_set_style_local_bg_opa(bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, v);
        }

        void static mbox_event_cb(lv_obj_t *obj, lv_event_t evt) {
            MessageBox* mb = (MessageBox*) lv_obj_get_user_data(obj);
            if (evt == LV_EVENT_VALUE_CHANGED) { // Button was clicked
                lv_msgbox_start_auto_close(obj, 0);
                //Serial.println("mbox_event_cb Button pressed, start");
                uint16_t btn = lv_msgbox_get_active_btn(obj);
                //Serial.printf("Button %d was pressed with mb=%p\n", btn, mb);
                if (btn!=LV_BTNMATRIX_BTN_NONE) {
                    if (mb->callback!=nullptr) {
                        //Serial.println("Calling callback");
                        // Callback with Button number
                        (mb->callback)(btn);
                    }
                }
                //Serial.printf("Starting delete of mbox, mb=%p\n", mb);
                lv_obj_del(mb->mbox);
                //Serial.printf("Starting delete of background, mb=%p\n", mb);
                lv_obj_del(mb->bg);
                //Serial.println("nullptr-izing stuff");
                mb->mbox = nullptr;
                mb->bg = nullptr;
            } else  if (evt == LV_EVENT_DELETE) {
                if (mb->mbox!=nullptr) {
                    lv_obj_del(mb->mbox);
                    mb->mbox=nullptr;
                }
                if (mb->bg!=nullptr) {
                    lv_obj_del(mb->bg);
                    mb->bg=nullptr;
                }
            }

        }

        void setup() {
            //Serial.println("Styles::MessageBox::setup() start");
            lv_style_init(&bgStyle); 
            lv_style_set_bg_color(&bgStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            //Serial.println("Styles::MessageBox::setup() end");
        }
        
    } messageBox;

    /**
     * Initialize all styles, must be called on setup.
     */
    void setup() {
        lv_style_init(&background);
        /*lv_style_set_bg_color(&background, LV_STATE_DEFAULT,  LV_COLOR_MAKE(0,0,100));
        lv_style_set_bg_grad_color(&background, LV_STATE_DEFAULT,LV_COLOR_MAKE(100,0,0));
        lv_style_set_bg_grad_dir(&background, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);*/
        lv_style_set_bg_opa(&background, LV_OBJ_PART_MAIN, LV_OPA_0);
        lv_style_set_border_width(&background, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_top(&background, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_bottom(&background, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_left(&background, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_right(&background, LV_STATE_DEFAULT, 0);
        lv_style_set_radius(&background, LV_OBJ_PART_MAIN, 0);

        lv_style_init(&titleLabel);
        lv_style_set_text_color(&titleLabel, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xff, 0x60, 0xc0));
        lv_style_set_text_font(&titleLabel, LV_STATE_DEFAULT, &ubuntu_r_28);
        
        lv_style_init(&buttonLabel);
        lv_style_set_text_color(&buttonLabel, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xb0, 0xb0, 0xff));
        lv_style_set_text_font(&buttonLabel, LV_STATE_DEFAULT, &ubuntu_r_22);
        
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
        lv_style_set_text_font(&textLabel, LV_STATE_DEFAULT, &ubuntu_r_22);

        lv_style_init(&appTileStyle);
        lv_style_set_radius(&appTileStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_bg_opa(&appTileStyle, LV_OBJ_PART_MAIN, LV_OPA_0);
        lv_style_set_border_width(&appTileStyle, LV_OBJ_PART_MAIN, 0);
        lv_style_set_text_color(&appTileStyle, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);

        messageBox.setup();
        
    }    

    

};


/**
 * Global Styles object
 */
extern Styles styles;