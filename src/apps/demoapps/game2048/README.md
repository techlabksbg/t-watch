# Boilerplate for 2048 

All the boilerplate code for 2048 is already provided. What is completely missing the
game logic.

You have to complete the following method (at the very bottom of the code)

   void Game2048::click(int x, int y)

`x` and `y` are the coordinates, where the user has touched the screen. Use them to
decide in which direction the tiles have to be moved.

The state of the board is stored in the variable `feld`, which is a one-dimensional array with 16 entries.
It contains 0 for empty tiles. Or it contains the exponent for other tiles (3 means 2^3=8).
You can access this variable using x,y-coordinates:
  - `byte Game2048::getFeld(int x, int y)`
  - `void Game2048::setFeld(int x, int y, byte value)`
These methods perform range checking but do not report out of bounds messages. `getFeld` returns 0 for out of bounds coordinates.


