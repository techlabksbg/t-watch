/**
 * @author Ivo Blöchliger
 */

#include "sudokuapp.h"


bool SudokuApp::create() {

    return true;
}

bool SudokuApp::show() {
    sudoku = new Sudoku();
    sudoku->generate();
    sudoku->toSerial();
    return true;
}
bool SudokuApp::hide() {
    delete sudoku;
    return true;
}
bool SudokuApp::destroy() {
    return true;
}

