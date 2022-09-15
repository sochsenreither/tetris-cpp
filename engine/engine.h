#pragma once

#include <SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <string>

#include "../game/game.h"

const auto scale = 30;
const auto width = 2014;
const auto height = 900;
const auto x_offset = width / 2 - COLS * scale / 2;
const auto y_offset = height / 2 - ROWS * scale / 2;
const auto preview = true;

struct Engine {
    // TODO: add ai
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    TTF_Font *font_small;
    Game game{};
    bool pause{false};
    bool running{true};

    std::string get_player_move();

    void render();

    void render_next_piece();

    void render_pause();

    void render_game_over();

    void render_text_box();

    void render_text();

    void render_canvas_block();

    void render_block();

    void render_stats();

public:
    Engine();

    ~Engine();

    void run();
};