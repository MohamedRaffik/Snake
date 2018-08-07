#ifndef __SNAKEGAME_H__
#define __SNAKEGAME_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include "textbox.h"
#include "gameblocks.h"

using namespace std;


typedef enum
{
    HOME = 0,
    OPTIONS,
    HTP,
    GAME,
    QUIT,
} MenuCodes;


class Snake_Game
{
public:

    Snake_Game();

    bool init();

    MenuCodes home_menu();

    MenuCodes option_menu();

    MenuCodes howtoplay_menu();

    bool play_again();

    int pick_color();

    int begin_game();

    int game_loop();

    void fps_cap(Uint32 start, Uint32 end, int frames);

    GameBlock point_block(SDL_Rect & rectangle);

    void reset();

    void exit();

private:

    string int_to_string(int num);
    string adjust_score_string(int num);
    void change_resolution(string new_resolution);
    void clear_screen();

    SDL_Window * main_Window;
    SDL_Surface * main_Surface;
    SDL_Renderer * main_Renderer;

    int window_width;
    int window_height;

    string resolutions[5];
    int R;

    SDL_Color colors[3];
    int C;

    SDL_Color Black;
    SDL_Color White;
    SDL_Color Light_Green;
    SDL_Color Yellow;

    int current_score;
    int high_score;
};

#endif // __SNAKEGAME_H__
