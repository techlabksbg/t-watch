/**
 * @author Ivo Blöchliger
 */

#include "credits.h"

bool Credits::create() {
    #include "credits.txt"
    lv_obj_t* page = styles.stdPage(myScr);
    lv_obj_t* t = styles.stdTitle(page, "Code by");
    lv_obj_align(t, page, LV_ALIGN_IN_TOP_LEFT, 5,5);
    lv_obj_t* l = styles.stdLabel(page, coders);
    lv_obj_align(l, t, LV_ALIGN_OUT_BOTTOM_LEFT, 0,5);
    t = styles.stdTitle(page, "Artwork by");
    lv_obj_align(t, l, LV_ALIGN_OUT_BOTTOM_LEFT, 0,5);
    l = styles.stdLabel(page, artists);
    lv_obj_align(l, t, LV_ALIGN_OUT_BOTTOM_LEFT, 0,5);
    return true;
}