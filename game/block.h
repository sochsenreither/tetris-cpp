#pragma once

#include <SDL.h>

const int ROWS = 22;
const int COLS = 10;

struct Block {
    int row{};
    int col{};
    SDL_Color color{};
    bool active{};
};