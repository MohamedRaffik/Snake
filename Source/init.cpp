#include "init.h"

bool init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { return false; }

    if (TTF_Init() < 0) { return false; }
}
