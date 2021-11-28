/**
 * @author Sebastian Gfeller
 */

#include "tide.h"

bool Tide::create() {
    register_for_swipe_up(myScr);
    lv_obj_add_style(myScr, LV_OBJ_PART_MAIN, &styles.background);

    highTitle = lv_label_create(myScr, NULL);
    lv_obj_add_style(highTitle, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_label_set_text(highTitle, "High Tide");

    highLabel = lv_label_create(myScr, NULL);
    lv_obj_add_style(highLabel, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_obj_align(highLabel, highTitle, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 24);
    lv_label_set_text(highLabel, " - ");

    lowTitle = lv_label_create(myScr, NULL);
    lv_obj_add_style(lowTitle, LV_OBJ_PART_MAIN, &styles.titleLabel);
    lv_obj_align(lowTitle, highLabel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 24);
    lv_label_set_text(lowTitle, "Low Tide");

    lowLabel = lv_label_create(myScr, NULL);
    lv_obj_add_style(lowLabel, LV_OBJ_PART_MAIN, &styles.textLabel);
    lv_obj_align(lowLabel, lowTitle, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 24);
    lv_label_set_text(lowLabel, " - ");

    Serial.println("Create tide");

    return true;
}

bool Tide::show() {
    Serial.print("Show tide");

    HTTPClient client;

    client.begin("http://tersetide.azurewebsites.net/api/tideinformation");
    client.GET();

    String informationString = client.getString();

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, informationString);

    const char* highString = doc["high"];
    const char* lowString = doc["low"];

    Serial.print("High ");
    Serial.print(highString);
    Serial.print("Low");
    Serial.print(lowString);

    lv_label_set_text(highLabel, highString);
    lv_label_set_text(lowLabel, lowString);

    client.end();

    return true;
}

bool Tide::hide() {
    return true;
}
