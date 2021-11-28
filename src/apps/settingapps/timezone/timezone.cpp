#include "timezone.h"

#include "../../../services/services.h"
#include "../../../os/certs/certs.h"


#define TIMEZONES_FILE "/timezones.txt"
#define BUFSIZE 10000
#define BTN_MAP_SIZE 1000

bool TimeZone::create() {
    return true;
}

bool TimeZone::getMemory() {
    if (btn_map==nullptr) {
        try {
            btn_map = new char*[BTN_MAP_SIZE];
        } catch (std::bad_alloc& ba) {
            Serial.println("new failed in TimeZone::getMemory()");
            btn_map = nullptr;
            return false;
        }
    }
    if (buf==nullptr) {
        try {
            buf = new char[BUFSIZE];
        } catch (std::bad_alloc& ba) {
            Serial.println("new failed in TimeZone::getMemory()");
            delete[] btn_map;
            btn_map = nullptr;
            buf = nullptr;
            return false;
        }
    }
    return true;
}

void TimeZone::freeMemory() {
    if (btn_map!=nullptr) {
        delete[] btn_map;
        btn_map = nullptr;
    }
    if (buf!=nullptr) {
        delete[] buf;
        buf = nullptr;
    }
}

bool TimeZone::makeRegionButtons() {
    if (!getMemory()) return false;
    fs::File file = SPIFFS.open(TIMEZONES_FILE,"r");
    int btnc = 0;  // number of buttonmap entries
    int bufi = 0;  // number of characters in buffer.
    while(file.available()) {
        String line = file.readStringUntil('\n');  
        if (line.charAt(0)!=' ') {
            //Serial.println(line);
            strcpy(buf+bufi, line.c_str());
            btn_map[btnc++] = buf+bufi;
            bufi+=line.length()+1;
            //Serial.printf("line[0]=%d, btn_map[%d]=%s\n", line.charAt(0), btnc-1, btn_map[btnc-1]);
            btn_map[btnc++] = buf+bufi;
            buf[bufi++]='\n';
            buf[bufi++]=0;
            //Serial.printf("btn_map[%d]=%s\n", btnc-1, btn_map[btnc-1]);
        }
    }
    btn_map[btnc++] = buf+bufi-1;  // Last string must be ""
    //Serial.printf("Last entry ->%s<-\n",btn_map[btnc-1]);
    file.close();
    btn_matrix = lv_btnmatrix_create(page, NULL);
    lv_obj_set_width(btn_matrix,220);
    lv_obj_set_height(btn_matrix,30*btnc);
    lv_btnmatrix_set_map(btn_matrix, (const char**) btn_map);
    lv_page_glue_obj(btn_matrix, true);
    lv_obj_align(btn_matrix, page, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_user_data(btn_matrix, this);
    lv_obj_set_event_cb(btn_matrix, [](lv_obj_t * obj, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            char region[30];
            strncpy(region, lv_btnmatrix_get_active_btn_text(obj),29);
            TimeZone* timeZone = (TimeZone*)lv_obj_get_user_data(obj);
            lv_obj_del(timeZone->btn_matrix);
            timeZone->btn_matrix = nullptr;
            //Serial.printf("Getting region %s\n", region);
            timeZone->makeCitiesButtons(region);
        }
    });
    return true;
}

void TimeZone::makeCitiesButtons(const char* region) {
    fs::File file = SPIFFS.open(TIMEZONES_FILE,"r");
    int btnc = 0;  // number of buttonmap entries
    int bufi = 0;  // number of characters in buffer.
    bool inSection = false;
    while(file.available()) {
        String line = file.readStringUntil('\n');  
        if (line.charAt(0)!=' ') {
            if (line==region && !inSection) {
                inSection = true;
            } else if (inSection) {
                break;
            }
        } else if (inSection && line.charAt(1)!=' ') {
            //Serial.println(line);
            btn_map[btnc++] = buf+bufi;
            strcpy(buf+bufi, line.c_str()+1);
            bufi+=line.length();
            btn_map[btnc++] = buf+bufi;
            buf[bufi++]='\n';
            buf[bufi++]=0;
        }
    }
    btn_map[btnc++] = buf+bufi-1;  // Last string must be ""
    file.close();
    btn_matrix = lv_btnmatrix_create(page, NULL);
    lv_obj_set_width(btn_matrix,220);
    lv_obj_set_height(btn_matrix,20*btnc);
    lv_btnmatrix_set_map(btn_matrix, (const char**) btn_map);
    lv_page_glue_obj(btn_matrix, true);
    lv_obj_align(btn_matrix, page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_user_data(btn_matrix, this);
    lv_obj_set_event_cb(btn_matrix, [](lv_obj_t * obj, lv_event_t event) {
        if (event == LV_EVENT_CLICKED) {
            char city[60];
            strncpy(city, lv_btnmatrix_get_active_btn_text(obj),59);
            TimeZone* timeZone = (TimeZone*)lv_obj_get_user_data(obj);
            lv_obj_del(timeZone->btn_matrix);
            timeZone->btn_matrix = nullptr;
            //Serial.println(city);
            timeZone->setTimeZone(city);
        }
    });
}

void TimeZone::setTimeZone(const char* city) {
    fs::File file = SPIFFS.open(TIMEZONES_FILE,"r");
    while(file.available()) {
        String line = file.readStringUntil('\n');
        if (line.substring(1) == city) {
            line = file.readStringUntil('\n');
            const char* tzinfo = line.c_str()+2;
            Serial.printf("Info for city %s is ->%s<-\n", city, tzinfo);
            (*configJson)["tz"] = tzinfo;
            saveJsonConfig();
            configTzTime(tzinfo, "pool.ntp.org");
            break;
        }
    }
    file.close();
    hide_myself();
}

bool TimeZone::show() {
    page = styles.stdPage(myScr);
    lv_obj_align(page, myScr, LV_ALIGN_CENTER, 0,0);

    if (downloadToSPIFFS(this, TIMEZONES_FILE, "https://tech-lab.ch/twatch/timezones.txt", ISRG_ROOT_X1)) {
        if (!makeRegionButtons()) return false;
    }
    return true;
}

bool TimeZone::hide() {
    if (page!=nullptr) {
        lv_obj_del(page);
    }
    freeMemory();
    return true;
}

bool TimeZone::destroy() {
    return true;
}



