/**
 * @author Ivo Blöchliger
 */

#include "minesweeper.h"


Minesweeper::Minesweeper(): size{12} {}
const char* Minesweeper::staticLabels[] = {" ", "1", "2", "3", "4", "5", "6", "7", "8", LV_SYMBOL_CLOSE};


bool Minesweeper::create() {
    byte* temp = new byte[size*size];
    feld = new byte*[size];
    for (int i=0; i<size; i++) {
        feld[i]=temp+i*size;
    }
    buttons = new lv_obj_t*[size*size];
    for (int i=0; i<size*size; i++) { buttons[i] = nullptr;}
    page = styles.stdPage(myScr, false);

    initStyles();

    initFeld();

    return true;
}

void Minesweeper::initStyles() {
    lv_style_init(&hiddenStyle);
    lv_style_set_bg_color(&hiddenStyle, LV_STATE_DEFAULT,  LV_COLOR_MAKE(100,100,180));
    lv_style_set_shadow_width(&hiddenStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_shadow_color(&hiddenStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(80,20,20));
    lv_style_set_shadow_ofs_x(&hiddenStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_shadow_ofs_y(&hiddenStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_border_width(&hiddenStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_top(&hiddenStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&hiddenStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&hiddenStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&hiddenStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&hiddenStyle, LV_OBJ_PART_MAIN, 0);


    lv_style_init(&shownStyle);
    lv_style_set_bg_color(&shownStyle, LV_STATE_DEFAULT,  LV_COLOR_MAKE(30,40,10));
    lv_style_set_shadow_width(&shownStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_shadow_color(&shownStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(20,80,20));
    lv_style_set_shadow_ofs_x(&shownStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_shadow_ofs_y(&shownStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_border_width(&shownStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_top(&shownStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&shownStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&shownStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&shownStyle, LV_STATE_DEFAULT, 0);

    lv_style_init(&labelStyle);
    lv_style_set_text_color(&labelStyle, LV_OBJ_PART_MAIN, LV_COLOR_MAKE(0xff, 0xff, 0xff));
    lv_style_set_text_font(&labelStyle, LV_STATE_DEFAULT, &ubuntu_r_28);
}

void Minesweeper::dumpFeld() {
    for (int y=0; y<size; y++) {
        for (int i=0; i<size; i++) Serial.print("+---");
        Serial.println('+');
        for (int x=0; x<size; x++) {
            Serial.print('|');
            Serial.print(feld[x][y]&16 ? '*' : ' ');
            if ((feld[x][y]&15)==0) Serial.print(' ');
            else if ((feld[x][y]&15)<9) Serial.print((char)('0'+((feld[x][y]&15))));
            else Serial.print('#');
            Serial.print(feld[x][y]&16 ? '*' : ' ');
        }
        Serial.println('|');
    }
    for (int i=0; i<size; i++) Serial.print("+---");
    Serial.println('+');
}

void Minesweeper::initFeld(int mines) {
    // All zero
    memset(feld[0],0,size*size);
    // Place 10 mines
    for (int i=0; i<mines;) {
        int x = random(size);
        int y = random(size);
        if (feld[x][y]!=9) {
            feld[x][y]=9;
            i++;
            for (int a=x-1;a<=x+1;a++) {
                if (a>=0 && a<size) {
                    for (int b=y-1;b<=y+1;b++) {
                        if (b>=0 && b<size) {
                            if (feld[a][b]!=9) {
                                feld[a][b]++;
                            }
                        }
                    }
                }
            }
        }
    }
    dumpFeld();
}

lv_obj_t* Minesweeper::createButton() {
    lv_obj_t* button = lv_btn_create(page, NULL);
    lv_obj_add_style(button, LV_OBJ_PART_MAIN, &hiddenStyle);
    lv_obj_set_size(button, 40, 40);
    lv_page_glue_obj(button, true);
    lv_obj_set_user_data(button, this);
    lv_obj_set_event_cb(button, button_cb);
    lv_obj_t* label = lv_label_create(button,NULL);
    lv_label_set_text(label, " ");
    lv_obj_add_style(label, LV_OBJ_PART_MAIN, &labelStyle);
    return button;
}

void Minesweeper::buildButtons() {
    if (buttons[0]!=nullptr) return;
    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            int i = y*size+x;
            buttons[i] = createButton();
            if (i==0) {
                lv_obj_align(buttons[i], page, LV_ALIGN_IN_TOP_LEFT, 0,0);
            } else {
                if (x==0) {
                    lv_obj_align(buttons[i], buttons[i-size], LV_ALIGN_OUT_BOTTOM_MID, 0,0);
                } else {
                    lv_obj_align(buttons[i], buttons[i-1], LV_ALIGN_OUT_RIGHT_MID, 0,0);
                }
            }
            setButton(x,y);
        }
    }
}

void Minesweeper::removeButtons() {
    if (buttons[0]==nullptr) return;
    for (int i=0; i<size*size; i++) {
        lv_obj_del(buttons[i]);
        buttons[i] = nullptr;
    }
}

void Minesweeper::newGame() {
    gameState = INIT;
    initFeld();
    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            setButton(x,y);
        }
    }
    gameState = PLAYING;
}

void Minesweeper::boom() {
    if (gameState == LOST) {
        newGame();
    } else {
        gameState = LOST;
        ttgo->motor->onec();
        // Callback in 1s
        lv_task_create([](lv_task_t* task) {
            ((Minesweeper*) (task->user_data))->boom();
            lv_task_del(task);
        }, 1000, LV_TASK_PRIO_LOWEST,  this);
    }
}

void Minesweeper::winner() {
    if (gameState == WON) {
        styles.hideSpinner();
        newGame();
    }else {
        gameState = WON;
        // Callback in 1s
        styles.showSpinner(myScr, "Winner!");
        lv_task_create([](lv_task_t* task) {
            ((Minesweeper*) (task->user_data))->winner();
            lv_task_del(task);
        }, 2000, LV_TASK_PRIO_LOWEST,  this);
    }
}

void Minesweeper::setButton(int x, int y) {
    int i = y*size+x;
    int v = feld[x][y];
    lv_obj_t* label = lv_obj_get_child(buttons[i], nullptr);
    if (v&32) { // marked?
        lv_obj_add_style(buttons[i], LV_OBJ_PART_MAIN, &shownStyle);
        lv_label_set_text(label, staticLabels[9]);
    } else { // not marked
        if (v&16) { // value shown
            lv_obj_add_style(buttons[i], LV_OBJ_PART_MAIN, &shownStyle);
            lv_label_set_text(label, staticLabels[v&15]);
            //lv_btn_set_state(buttons[i], LV_BTN_STATE_DISABLED);    
        } else { // value hidden
            lv_obj_add_style(buttons[i], LV_OBJ_PART_MAIN, &hiddenStyle);
            lv_label_set_text(label, " ");
            //lv_btn_set_state(buttons[i], LV_BTN_STATE_RELEASED);   
        }
    }
    bool won = true;
    for (int y=0; y<size; y++) {
        for (int x=0; x<size; x++) {
            if ((feld[x][y])==9) { // unmarked mine
                won = false;
                break;
            }
        }
        if (!won) break;
    }
    if (won) {
        winner();
    }
}

void Minesweeper::uncover(int x, int y) {
    if (feld[x][y]==0) {
        feld[x][y] = 16;
        setButton(x,y);
        for (int a=x-1; a<=x+1; a++) {
            if (a>=0 && a<size) {
                for (int b=y-1; b<=y+1; b++) {
                    if(b>=0 && b<size) {
                        if ((feld[a][b]&48)==0) {
                            if (feld[a][b]==0) {
                                uncover(a,b);
                            } else {
                                feld[a][b]|=16;
                                setButton(a,b);
                            }
                        }
                    }
                }
            }
        }
        return;
    }
    if (feld[x][y]&16) {  // Long click on an shown field.
        int v = feld[x][y]&15;
        if (v>0 && v<9) { // really a number field?
            Serial.println("Counting...");
            int c = 0;
            for (int a=x-1; a<=x+1; a++) {
                if (a>=0 && a<size) {
                    for (int b=y-1; b<=y+1; b++) {
                        if(b>=0 && b<size) {
                            if (a!=x || b!=y) {
                                if (feld[a][b]&32) {
                                    c++;
                                }
                            }
                        }
                    }
                }
            }
            Serial.printf("Got %d\n", c);
            if (c==v) { // same number of marked fields around as value on field
                for (int a=x-1; a<=x+1; a++) {
                    if (a>=0 && a<size) {
                        for (int b=y-1; b<=y+1; b++) {
                            if(b>=0 && b<size) {
                                if (a!=x || b!=y) {
                                    if ((feld[a][b]&48)==0) { // field is still hidden and not marked
                                        if (feld[a][b]==0) {
                                            uncover(a,b);
                                        } else {
                                            if (feld[a][b]==9) {
                                                boom();
                                            } else {
                                                feld[a][b]|=16;
                                                setButton(a,b);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Minesweeper::buttonClicked(lv_obj_t* button, bool shortClick) {
    if (gameState!=PLAYING) return; // not currently playing.
    int i=0;
    for(;(buttons[i]!=button);i++);
    int x = i%size;
    int y = i/size;
    if (feld[x][y]&16) { // shown field
        if (shortClick) return; // already shown, nothing to do
        // long click it is
        if (feld[x][y]&32) {
            feld[x][y]^=48;
            setButton(x,y);
        }
        if ((feld[x][y]&15)>0 && (feld[x][y]&15)<9) { // open number field
            Serial.println("uncover on shown number field");
            uncover(x,y);
        }
        return;
    } 
    // field is not shown
    if (!shortClick) {
        feld[x][y]^=48;
        setButton(x,y);
    } else { // short click
        if (feld[x][y]==0) {
            uncover(x,y);
        } else {
            if (feld[x][y]==9) {
                boom();
            }
            feld[x][y]|=16;
            setButton(x,y);
        }
    }
}

bool Minesweeper::show() {
    gameState=INIT;
    buildButtons();
    gameState=PLAYING;
    return true;
}
bool Minesweeper::hide() {
    removeButtons();
    return true;
}
bool Minesweeper::destroy() {
    if (feld!=nullptr) {
        delete[] feld[0];
        delete[] feld;
        feld = nullptr;
    }
    if (buttons!=nullptr) {
        delete[] buttons;
    }
    return true;
}

