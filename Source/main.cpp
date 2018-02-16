#include "snakegame.h"

using namespace std;

int main()
{
    Snake_Game Game;

    if (!Game.init()) { return -1; }

    while (Game.game_loop()) {}

    Game.exit();

    return 0;
}

//Rename MenuOption to TextBox
