#include "shoppinglist.h"

bool ShoppingList::create() {
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);

    Serial.println("Try log shop");

    return true;
}

bool ShoppingList::show() {
    Serial.println("Try show shop");

    return true;
}