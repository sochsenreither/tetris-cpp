#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "../game/game.h"
#include "../ai/ai.h"

const auto SCALE = 30;
const auto WIDTH = 1024;
const auto HEIGHT = 900;
const auto X_OFFSET = WIDTH / 2 - COLS * SCALE / 2;
const auto Y_OFFSET = HEIGHT / 2 - ROWS * SCALE / 2;

class Engine {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    TTF_Font *font_small;
    TetrisPlayer ai;
    Game game{};
    bool pause{false};
    bool running{true};

    std::string get_player_move();

    void render();

    void render_next_piece();

    void render_text_box(const char *text);

    void render_text(const char *text, int x, int y, TTF_Font *fnt);

    void render_canvas_block(int x, int y);

    void render_block(int x, int y, SDL_Color color);

    void render_stats();

public:
    Engine();

    ~Engine();

    void run();

    void run_ai();
};