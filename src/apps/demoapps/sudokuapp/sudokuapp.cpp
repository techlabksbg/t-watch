/**
 * @author Ivo Blöchliger
 */

#include "sudokuapp.h"

static const char* numbers[]={"", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

bool SudokuApp::create() {
    // General background to black
    lv_obj_set_style_local_bg_color(myScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    createStyles();
    sudoku = new Sudoku();
    sudoku->generate();
    sudoku->toSerial();
    return true;
}

void SudokuApp::onClick(lv_obj_t *obj) {
    //Serial.println("onClick");
    // Check if click on main grid:
    for (int i=0; i<9; i++) {
        if (obj == grid[i]) {
            if (state==NORMAL) {
                createOverlay(i%3, i/3);
            } else if (state==OVERLAY) {
                destroyOverlay();
            } else {
                destroyNumberPad();
                destroyOverlay();
            }
            return;
        }
    }
    if (state==NORMAL) return; // No more objects to check
    // Check if click on Overlay
    for (int i=0; i<9; i++) {
        if (obj == oentries[i]) {
            if (state==OVERLAY) {
                createNumberPad(i);
            } else {
                destroyNumberPad();
            }
            return;
        }
    }
    if (state==OVERLAY) return;
    for (int i=0; i<11; i++) {
        if (obj == kentries[i]) {
            if (i>=0 && i<=9) {  // 0 means clear ;-)
                setNumber(activex, activey, i);
                destroyNumberPad();
                destroyOverlay();
                if (sudoku->isSolved()) {
                    ttgo->motor->onec();
                    delete sudoku;
                    sudoku = new Sudoku();
                    sudoku->generate();
                    deleteGUI();
                    createGUI();
                }
            } else if (i==10) {
                // new sudoku
                destroyNumberPad();
                destroyOverlay();
                delete sudoku;
                sudoku = new Sudoku();
                sudoku->generate();
                deleteGUI();
                createGUI();
            }
        }
    }
}

void SudokuApp::setClick(lv_obj_t* obj) {
    lv_obj_set_user_data(obj, this);
    lv_obj_set_event_cb(obj, [](lv_obj_t *obj, lv_event_t event) {
        if (event != LV_EVENT_SHORT_CLICKED) return;
        ((SudokuApp*)lv_obj_get_user_data(obj))->onClick(obj);
    });
}

void SudokuApp::createStyles() {
    grid_style = new lv_style_t;
    lv_style_init(grid_style);
    lv_style_set_border_color(grid_style, LV_STATE_DEFAULT, LV_COLOR_MAKE(200,200,200));
    lv_style_set_border_width(grid_style, LV_STATE_DEFAULT, 1);
    lv_style_set_bg_color(grid_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    entries_style = new lv_style_t;
    lv_style_init(entries_style);
    lv_style_set_border_color(entries_style, LV_STATE_DEFAULT, LV_COLOR_MAKE(50,50,50));
    lv_style_set_border_width(entries_style, LV_STATE_DEFAULT, 1);
    lv_style_set_bg_color(entries_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    keypad_style = new lv_style_t;
    lv_style_copy(keypad_style, grid_style);
    lv_style_set_bg_color(keypad_style, LV_STATE_DEFAULT, LV_COLOR_MAKE(50,60,70));


    labels_style = new lv_style_t;
    labelsfix_style = new lv_style_t;
    lv_style_init(labels_style);
    lv_style_set_text_color(labels_style, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_font(labels_style, LV_STATE_DEFAULT, &ubuntu_r_22);
    lv_style_copy(labelsfix_style, labels_style);
    lv_style_set_text_color(labelsfix_style, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(200,180,100));


    labelsmed_style = new lv_style_t;
    labelsfixmed_style = new lv_style_t;
    lv_style_init(labelsmed_style);
    lv_style_set_text_color(labelsmed_style, LV_OBJ_PART_MAIN, LV_COLOR_WHITE);
    lv_style_set_text_font(labelsmed_style, LV_STATE_DEFAULT, &ubuntu_r_28);
    lv_style_copy(labelsfixmed_style, labelsmed_style);
    lv_style_set_text_color(labelsfixmed_style, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(200,180,100));

}
void SudokuApp::deleteStyles() {
    delete grid_style;
    delete entries_style;
    delete labels_style;
    delete labelsmed_style;
}

void SudokuApp::createGUI() {
    grid = new lv_obj_t*[9];
    // Main grid
    for (int i=0; i<9; i++) {
        grid[i] = lv_obj_create(myScr, nullptr);
        lv_obj_set_size(grid[i], 80,80);
        lv_obj_add_style(grid[i], LV_STATE_DEFAULT, grid_style);
        lv_obj_align(grid[i], nullptr, LV_ALIGN_IN_TOP_LEFT, (i%3)*80, (i/3)*80);
        setClick(grid[i]);
    }
    entries = new lv_obj_t*[81];
    labels = new lv_obj_t*[81];
    for (int i=0; i<81; i++) {
        entries[i] = lv_obj_create(grid[(i%9)/3+i/27*3], nullptr);
        lv_obj_set_click(entries[i], false);
        lv_obj_set_size(entries[i], 26,26);
        lv_obj_add_style(entries[i], LV_STATE_DEFAULT, entries_style);
        int xoff = 1+(i%3)*26;
        int yoff = 1+((i/9)%3)*26;
        lv_obj_align(entries[i], nullptr, LV_ALIGN_IN_TOP_LEFT, xoff, yoff);

        labels[i] = lv_label_create(entries[i],NULL);
        lv_label_set_text(labels[i], numbers[sudoku->get(i%9, i/9)]);
        lv_obj_add_style(labels[i], LV_OBJ_PART_MAIN, sudoku->isFixed(i%9, i/9) ? labelsfix_style : labels_style);
        lv_obj_align(labels[i], nullptr, LV_ALIGN_CENTER, 0, 0);
    }
}
void SudokuApp::deleteGUI() {
    if (state != NORMAL) {
        destroyOverlay();
    }
    for (int i=0; i<9; i++) {
        lv_obj_del(grid[i]);
    }
    delete[] labels;
    delete[] entries;
    delete[] grid;
}

void SudokuApp::setNumber(int x, int y, byte v) {
    Serial.printf("setNumber(%d,%d) to %d\n", x,y,v);
    if (!sudoku->isFixed(x,y)) {
        Serial.println("   done");
        sudoku->set(x,y,v);
        lv_label_set_text(labels[x+y*9], numbers[v]);
        lv_obj_align(labels[x+y*9], nullptr, LV_ALIGN_CENTER, 0, 0);
    }
}

void SudokuApp::createOverlay(int x, int y) {
    state = OVERLAY;
    activex = x;
    activey = y;
    obase = lv_obj_create(myScr, nullptr);
    lv_obj_set_size(obase, 183,183);
    lv_obj_add_style(obase, LV_STATE_DEFAULT, grid_style);
    lv_obj_align(obase, nullptr, LV_ALIGN_IN_TOP_LEFT, x*20+8, y*20+8);
    lv_obj_set_click(obase, false);

    oentries = new lv_obj_t*[9];
    olabels = new lv_obj_t*[9];
    // Sub grid
    for (int i=0; i<9; i++) {
        oentries[i] = lv_obj_create(obase, nullptr);
        lv_obj_set_size(oentries[i], 60,60);
        lv_obj_add_style(oentries[i], LV_STATE_DEFAULT, entries_style);
        lv_obj_align(oentries[i], nullptr, LV_ALIGN_IN_TOP_LEFT, (i%3)*61+1, (i/3)*61+1);
        if (sudoku->isFixed(x*3+i%3, y*3+i/3)) {
            lv_obj_set_click(oentries[i], false);
        } else {
            setClick(oentries[i]);
        }

        olabels[i] = lv_label_create(oentries[i],NULL);
        lv_label_set_text(olabels[i], numbers[sudoku->get(x*3+i%3, y*3+i/3)]);
        lv_obj_add_style(olabels[i], LV_OBJ_PART_MAIN, sudoku->isFixed(x*3+i%3, y*3+i/3) ? labelsfixmed_style : labelsmed_style);
        lv_obj_align(olabels[i], nullptr, LV_ALIGN_CENTER, 0, 0);
    }
}
void SudokuApp::destroyOverlay() {
    if (state == KEYPAD) {
        destroyNumberPad();
    }
    state = NORMAL;
    lv_obj_del(obase);
    delete[] oentries;
    delete[] olabels;
}

void SudokuApp::createNumberPad(int subindex) {
    state = KEYPAD;
    activex = activex*3 + subindex%3;
    activey = activey*3 + subindex/3;
    kbase = lv_obj_create(myScr, nullptr);
    lv_obj_set_size(kbase, 240,240);
    lv_obj_add_style(kbase, LV_STATE_DEFAULT, grid_style);
    lv_obj_align(kbase, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    kentries = new lv_obj_t*[16];
    klabels = new lv_obj_t*[16];
    // Sub grid
    for (int i=0; i<11; i++) {
        kentries[i] = lv_obj_create(kbase, nullptr);
        lv_obj_add_style(kentries[i], LV_STATE_DEFAULT, keypad_style);
        klabels[i] = lv_label_create(kentries[i],NULL);
        lv_obj_add_style(klabels[i], LV_OBJ_PART_MAIN, labelsmed_style);
        if (i==0) {
            lv_obj_set_size(kentries[i], 180,60);
            lv_obj_align(kentries[i], nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 0);
            lv_label_set_text(klabels[i], "clear");
        } else if (i>0 && i<10) {
            lv_obj_set_size(kentries[i], 60,60);
            lv_obj_align(kentries[i], nullptr, LV_ALIGN_IN_TOP_LEFT, ((i-1)%3)*60, ((i-1)/3)*60+60);
            lv_label_set_text(klabels[i], numbers[i]);
        } else if (i==10) {
            lv_obj_set_size(kentries[i], 60,60);
            lv_obj_align(kentries[i], nullptr, LV_ALIGN_IN_TOP_LEFT, 180, 0);
            lv_label_set_text(klabels[i], LV_SYMBOL_REFRESH);
        }
        setClick(kentries[i]);
        lv_obj_align(klabels[i], nullptr, LV_ALIGN_CENTER, 0, 0);
    }
    lv_obj_set_click(kbase, false);

}
void SudokuApp::destroyNumberPad() {
    state = OVERLAY;
    lv_obj_del(kbase);
    delete[] klabels;
    delete[] kentries;
}

bool SudokuApp::show() {
    state = NORMAL;
    createGUI();
    return true;
}
bool SudokuApp::hide() {
    deleteGUI();
    return true;
}
bool SudokuApp::destroy() {
    deleteStyles();
    delete sudoku;
    return true;
}

