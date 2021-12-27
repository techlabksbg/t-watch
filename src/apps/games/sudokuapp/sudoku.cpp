/**
 * author Ivo Blöchliger
 */

#include "sudoku.h"

Sudoku::Sudoku() {
    field = new byte*[9];
    fixed = new bool*[9];
    for (int x=0; x<9; x++) {
        field[x] = new byte[9];
        fixed[x] = new bool[9];
        for (int y=0; y<9; y++) {
            field[x][y] = 0;
            fixed[x][y] = false;
        }
    }
}
Sudoku::~Sudoku() {
    for (int x=0; x<9; x++) {
        delete[] field[x];
        delete[] fixed[x];
    }
    delete[] field;
    delete[] fixed;
}

void Sudoku::clear() {
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {
            field[x][y] = 0;
            fixed[x][y] = false;
        }
    }
}

void Sudoku::set(int x, int y, byte v) {
    field[x][y] = v;
}
byte Sudoku::get(int x, int y) {
    return field[x][y];
}
bool Sudoku::isFixed(int x, int y)  { return fixed[x][y]; }
bool Sudoku::isSolved() {
    // all filled?
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {
            if (field[x][y]==0) return false;
        }
    }
    byte num[9];
    // Vertical
    for (int x=0; x<9; x++) {
        memset(num,0,1);
        for (int y=0; y<9; y++) {
            byte v = field[x][y]-1;
            num[v]++;
            if (num[v]>1) return false;
        }
    }
    //horizontal
    for (int y=0; y<9; y++) {
        memset(num,0,1);
        for (int x=0; x<9; x++) {
            byte v = field[x][y]-1;
            num[v]++;
            if (num[v]>1) return false;
        }
    }
    //subsquares
    for (int y=0; y<9; y+=3) {
        for (int x=0; x<9; x+=3) {
            memset(num,0,1);
            for (int i=0; i<9; i++) {
                byte v = field[x+i%3][y+i/3]-1;
                num[v]++;
                if (num[v]>1) return false;
            }
        }
    }
    return true;
}

bool Sudoku::possible(int x, int y, byte v) {
    // Top left coordinates of subsquare
    int qx = x-x%3;
    int qy = y-y%3; 
    for (int i=0; i<9; i++) {
        // horizontal:
        if (field[i][y]==v) return false;
        // vertical:
        if (field[x][i]==v) return false;
        // subsquare
        if (field[qx+i%3][qy+i/3]==v) return false;
    }
    return true;
}

int Sudoku::numpos(int x, int y) {
    int r = 0;
    for (byte v=1; v<=9; v++) {
        if (possible(x,y,v)) r++;
    }
    return r;
}

byte Sudoku::next(int x, int y) {
    for (byte v = field[x][y]+1; v<=9; v++) {
        if (possible(x,y,v)) {
            set(x,y,v);
            return v;
        }
    }
    set(x,y,0);
    return 0;
}

int Sudoku::getMinPos() {
    int bestx;
    int besty;
    int bestn=10;
    int numbest=0;
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {
            if (field[x][y]==0) {
                int n = numpos(x,y);
                if (n==0) return -1; // Impossible Sudoku
                if (n<bestn) {
                    bestn = n;
                    numbest = 1;
                    bestx = x;
                    besty = y;
                } else if (n==bestn) {
                    if (random(numbest)==0) {
                        bestx = x;
                        besty = y;
                    }
                    numbest+=1;
                }
            }
        }
    }
    if (bestn==10) return -1;
    return bestx+9*besty;
}

void Sudoku::save(byte* buffer) {
    int i=0;
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {    
            buffer[i++] = field[x][y];
        }
    }
}

void Sudoku::load(byte* buffer) {
    int i=0;
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {    
            field[x][y] = buffer[i++];
        }
    }
}

int Sudoku::solve(bool singleSolution) {
    int positions[81];
    byte solution[81];
    int solved = 81;
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {    
            if (!fixed[x][y]) {
                field[x][y]=0;
                solved--;
            }
        }
    }
    int numberOfSolutions = 0;
    int posi = 0;
    int dir = 1;
    int p = 0;
    while (true) {
        //if (solved%9==0) toSerial();
        if (dir==1) { // find next good position
            p = getMinPos();
            //Serial.printf("%d from getMinPos()\n", p);
        } else {
            if (posi==0) {
                if (numberOfSolutions>0) {
                    load(solution);
                }
                return numberOfSolutions;
            }
            p = positions[--posi];
            //Serial.printf("%d from backtrack (depth now %d)", p, posi);
        }
        if (p!=-1) {
            int x = p%9;
            int y = p/9;
            // Set next possible value
            byte v = next(x,y);
            //Serial.printf("  %d as next value on %d,%d\n",v, x,y);
            if (v>0) {  // Still some value possible
                positions[posi++] = x+9*y;
                solved++;
                dir = 1;
                if (solved==81) {  // Solved?
                    save(solution);
                    //Serial.printf("  --> SOLVED solution %d\n",numberOfSolutions+1);
                    //toSerial();
                    if (singleSolution) return 1;
                    if (numberOfSolutions>0) return 2;
                    set(x,y,0);
                    solved--;
                    posi--;
                    numberOfSolutions++;
                    dir = -1;
                }
            } else { // No more value on this field? (happens when backtracking)
                dir = -1;
                solved--;
            }
        } else {  // Impossible? So backtrack!
            dir = -1;
        }
    }
}

void Sudoku::preProgrammed() {   
    byte fd[9][9] = {{5,1,0,0,0,0,0,0,3,},{4,9,0,2,1,0,8,0,0,},{3,0,6,4,8,0,1,9,0,},{0,0,0,3,0,0,5,1,8,},{0,3,0,8,9,0,0,0,0,},{0,8,0,0,0,7,0,0,0,},{7,0,0,0,0,4,9,0,0,},{0,5,3,9,0,1,0,0,2,},{9,4,0,0,0,8,0,3,0,},};
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {
            field[x][y] = fd[x][y];
            fixed[x][y] = field[x][y]!=0;
        }
    }
}

void Sudoku::generate() {
    clear();
    solve(true);
    //toSerial();
    // Fix all
    for (int x=0; x<9; x++) for (int y=0; y<9; y++) fixed[x][y] = true;
    bool mustfix[9][9];
    for (int x=0; x<9; x++) for (int y=0; y<9; y++) mustfix[x][y] = false;

    byte temp[81];
    save(temp);

    int ms = millis()+200;
    while (ms>millis()) {
        int x = random(9);
        int y = random(9);
        if (fixed[x][y] && !mustfix[x][y]) {
            fixed[x][y] = false;
            int n = solve(false);
            if (n>1) {
                fixed[x][y] = true;
                mustfix[x][y] = true;
                //Serial.printf("Must fix %d,%d\n",x,y);
            } else {
                //Serial.printf("Removed %d,%d\n",x,y);
            }
        }
    }
    load(temp);
    for (int x=0; x<9; x++) for (int y=0; y<9; y++) if (!fixed[x][y]) field[x][y] = 0;
    toCode();
}

void Sudoku::toCode() {
    Serial.print("byte fd[9][9] = {");
    for (int x=0; x<9; x++) {
        Serial.print('{');
        for (int y=0; y<9; y++) {
            Serial.printf("%d,",field[x][y]);
        }
        Serial.print("},");
    }
    Serial.println("};");
}

void Sudoku::toSerial() {
    for (int i=0; i<37; i++) Serial.print(i%4==0 ? '+' : '-');
    Serial.println();
    for (int y=0; y<9; y++) {
        Serial.print('|');
        for (int x=0; x<9; x++) {
            if (field[x][y]>0) {
                if (fixed[x][y]) {
                    Serial.printf("[%d]", field[x][y]);
                } else {
                    Serial.printf(" %d ", field[x][y]);
                }
            } else {
                Serial.print("   ");
            }
            Serial.print((x%3==2) ? '|' : ' ');
        }
        Serial.println();
        if (y%3==2) {
            for (int i=0; i<37; i++) Serial.print(i%4==0 ? '+' : '-');
        } else {
            for (int i=0; i<37; i++) Serial.print(i%4==0 ? '+' : ' ');
        }
        Serial.println();
    }
}