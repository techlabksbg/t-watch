#include "tide.h"

bool Tide::create() {
    register_for_swipe_up(myScr);

    Serial.println("Create tide");

    return true;
}

bool Tide::show() {
    Serial.print("Show tide");

    HTTPClient client;

    client.begin("http://tersetide.azurewebsites.net/api/tideinformation");
    client.GET();

    String highLowString = client.getString();
    Serial.print(highLowString);

    client.end();

    return true;
}

bool Tide::hide() {
    return true;
}
