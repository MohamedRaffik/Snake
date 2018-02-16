#ifndef __GAMEBLOCKS_H__
#define __GAMEBLOCKS_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "list.h"
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

typedef enum
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    NONE,
} Direction;

class GameBlock
{
public:

    GameBlock();

    GameBlock(SDL_Color & color, int x, int y, int w, int h);

    void Render(SDL_Renderer * renderer);

    void set_color(SDL_Color & new_color);

    SDL_Color get_color();

    void set_dimensions(int x, int y, int w, int h);

    SDL_Rect get_dimensions();

    Direction get_direction();

    void set_direction(Direction & new_movement);

    void clear_block(SDL_Renderer * renderer, SDL_Color & background);

private:

    SDL_Rect rectangle;
    SDL_Color Color;
    Direction movement;
};


class SnakeBlock
{
public:

    SnakeBlock(GameBlock & head_block);

    void Render(SDL_Renderer * renderer, SDL_Color & background, Direction & movement);

    void add_block();

    bool collect(GameBlock & point_block);

    bool collision(SDL_Rect & border);

    SDL_Color get_color();

    void set_color(SDL_Color & new_color);

private:

    void clear_snake(SDL_Renderer * renderer, SDL_Color & background);

    void move(Direction & movement);

    List<GameBlock> blocks;
};

#endif // __GAMEBLOCKS_H__


