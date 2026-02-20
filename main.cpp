/* ***********************************
 * Pathfinding Demo with C++/SFML
 * using SFML base template demo
 *
 * how to compile with cmake and CMakeLists.txt
 * ────────────────────────────────────────────
 * mkdir build directory, 
 * 1A. cd build && cmake ..
 * 1B. using cmake-gui (configure, generate)
 * 2.  cd build and make
 *********************************** */

#include <SFML/Graphics.hpp>

#include "Game.h"

int main()
{
    Game game;
    game.run();

    return 0;
}
