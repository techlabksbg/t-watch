/**
 * @author Ivo Blöchliger
 */

#include "minesweeper.h"


Minesweeper::Minesweeper(): size{12} {}

bool Minesweeper::create() {
    byte* temp = new byte[size*size];
    feld = new byte*[size];
    for (int i=0; i<size; i++) {
        feld[i]=temp+i*size;
    }
    return true;
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

bool Minesweeper::show() {
    initFeld();
    return true;
}
bool Minesweeper::hide() {
    return true;
}
bool Minesweeper::destroy() {
    if (feld!=nullptr) {
        delete[] feld[0];
        delete[] feld;
        feld = nullptr;
    }
    return true;
}
