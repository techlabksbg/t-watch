#include "sysinfo.h"

SysInfo* SysInfo::self;


bool SysInfo::create() {
    SysInfo::self = this;
    task = nullptr;
    page = styles.stdPage(myScr);

    label = lv_label_create(page, NULL);    
    lv_obj_add_style(label, LV_OBJ_PART_MAIN, &styles.textLabel);

    SysInfo::update(nullptr);

    lv_obj_align(label, page, LV_ALIGN_IN_TOP_MID, 0, 20);

    quitButton = styles.stdButton(page, "Quit");
    lv_obj_align(quitButton, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    register_for_hide_on_click(quitButton);

    return true;
}

bool SysInfo::show() {
    if (task==nullptr) {
        task = lv_task_create(SysInfo::update, 500, LV_TASK_PRIO_LOWEST, NULL);
    }
    return true;
}

bool SysInfo::hide() {
    if (task!=nullptr) {
        lv_task_del(task);
        task = nullptr;
    }
    return true;
}



