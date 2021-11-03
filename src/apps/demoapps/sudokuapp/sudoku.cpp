/**
 * author Ivo Blöchliger
 */

#include "sudoku.h"

// Constructor, creates an empty Sudoku
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
// Destructor, frees all allocated memory
Sudoku::~Sudoku() {
    for (int x=0; x<9; x++) {
        delete[] field[x];
        delete[] fixed[x];
    }
    delete[] field;
    delete[] fixed;
}

/**
 * TODO Clear all entries
 */
void Sudoku::clear() {
}


void Sudoku::set(int x, int y, byte v) {
    field[x][y] = v;
}
byte Sudoku::get(int x, int y) {
    return field[x][y];
}
bool Sudoku::isFixed(int x, int y)  { return fixed[x][y]; }
/**
 * TODO Check, if Sudoku is complelety and correctly solved
 */
bool Sudoku::isSolved() {
    return false;
}

/**
 * TODO check if the number v can be put at position x,y
 * with respect to basic rules (horizontal, vertical, subsquare)
 */
bool Sudoku::possible(int x, int y, byte v) {
    return true;
}

/**
 * TODO get the number of still possible entries at this position
 */
int Sudoku::numpos(int x, int y) {
    return 9;
}

/**
 * TODO find the next higher entry at this position an set it.
 * Set it to 0 if no such entry exists.
 * Return the value set.
 */
byte Sudoku::next(int x, int y) {
    return 0;
}

/**
 * TODO find a (preferably random) position with the least
 * number of possible entries.
 * Immediately return -1 if there is a position with no possible entries.
 * 
 * Return the position as a single number from 0 to 80 (x+9*y)
 */
int Sudoku::getMinPos() {
    return 0;
}

// Saves the current entries to a buffer
void Sudoku::save(byte* buffer) {
    int i=0;
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {    
            buffer[i++] = field[x][y];
        }
    }
}

// Loads the entries from a buffer
void Sudoku::load(byte* buffer) {
    int i=0;
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {    
            field[x][y] = buffer[i++];
        }
    }
}

/**
 * TODO solves a given Sudoku and returns the number of solutions.
 * it sets the current state to the first found solution
 * returns 2 in case of multiple solutions. 
 * returns 0 in case of no solutions
 * 
 * @param singleSolution when true, the search is stopped as soon as one solution has been found
 *      this can be used for generating a Sudoku
 */
int Sudoku::solve(bool singleSolution) {
    return 1;
}

/**
 * A progrogrammed Sudoku for Testing purposes
 */
void Sudoku::preProgrammed() {   
    byte fd[9][9] = {{5,1,0,0,0,0,0,0,3,},{4,9,0,2,1,0,8,0,0,},{3,0,6,4,8,0,1,9,0,},{0,0,0,3,0,0,5,1,8,},{0,3,0,8,9,0,0,0,0,},{0,8,0,0,0,7,0,0,0,},{7,0,0,0,0,4,9,0,0,},{0,5,3,9,0,1,0,0,2,},{9,4,0,0,0,8,0,3,0,},};
    for (int x=0; x<9; x++) {
        for (int y=0; y<9; y++) {
            field[x][y] = fd[x][y];
            fixed[x][y] = field[x][y]!=0;
        }
    }
}

/**
 * TODO
 * Generate a random completely filled sudoku. (with solve(true));
 * Remove clues, as long as the solution is unique.
 */
void Sudoku::generate() {
    preProgrammed();
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