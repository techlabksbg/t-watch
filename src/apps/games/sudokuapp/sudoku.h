/**
 * author Ivo Blöchliger
 */
#include "Arduino.h"

class Sudoku {
    public:
    Sudoku();
    ~Sudoku();

    void set(int x, int y, byte value);
    byte get(int x, int y);
    bool isSolved();

    // Sets all entries to 0, non-fixed
    void clear();

    int solve(bool singleSolution=false);
    void generate();
    void preProgrammed();
    
    // Can this value be set with respect to basic rules?
    bool possible(int x, int y, byte value);
    
    // Set the next possible value on this entry
    // @return 0 if no more entries possible
    byte next(int x, int y);

    // number of possible numbers in this case, with respect to basic rules
    int numpos(int x, int y);

    // returns a random position with the least number of possibles
    int getMinPos();

    // Output field to serial Port.
    void toSerial();
    void toCode();

    void save(byte* buffer);
    void load(byte* buffer);

    bool isFixed(int x, int y);

    private:
    // current state of the field?
    byte** field = nullptr;
    // Which cases are already fixed?
    bool** fixed = nullptr;


};