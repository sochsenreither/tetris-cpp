#include "engine.h"
#include <string>

Engine::Engine() {
    window = SDL_CreateWindow("tetris",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WIDTH,
                              HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        // TODO: throw exception
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        // TODO: throw exception
    }
    if (TTF_Init() < 0) {
        // TODO: throw exception
    }

    font = TTF_OpenFont("../font/SourceSansPro-Regular.otf", 48);
    if (font == nullptr) {
        // TODO: throw exception
    }
    font_small = TTF_OpenFont("../font/SourceSansPro-Regular.otf", 32);
    if (font_small == nullptr) {
        // TODO: throw exception
    }
}

Engine::~Engine() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_Quit();
}

void Engine::run() {
    auto counter = 0;

    while (running) {
        auto interval = 30 - (game.level * 3);
        if (interval < 5) interval = 5;

        SDL_SetRenderDrawColor(renderer, 35, 35, 35, 0);
        SDL_RenderClear(renderer);

        // Get player direction
        auto direction = get_player_move();

        if (direction == "RETURN" && game.game_over) {
            game = Game{};
            continue;
        }

        // Pass direction to game
        if (!pause && !game.game_over) {
            auto tick = false;
            if (++counter >= interval) {
                tick = true;
                counter = 0;
            }

            game.spawn_piece();
            game.step(direction, tick);
        }

        render();
        SDL_Delay(1);
    }
}

void Engine::run_ai() {
    auto ai_call = true;

    while (running) {
        SDL_SetRenderDrawColor(renderer, 35, 35, 35, 0);
        SDL_RenderClear(renderer);
        get_player_move();
        std::string direction = "DOWN";

        if (ai_call) {
            ai_call = false;
            game.spawn_piece();
            game.active_piece = ai.next_move(game.board, game.active_piece);
        }

        // Pass direction to game
        if (!pause && !game.game_over) {
            if (game.step(direction, true)) ai_call = true;
        }

        render();
        SDL_Delay(16);
    }
}

std::string Engine::get_player_move() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_UP:
                        return "UP";
                    case SDLK_DOWN:
                        return "DOWN";
                    case SDLK_RIGHT:
                        return "RIGHT";
                    case SDLK_LEFT:
                        return "LEFT";
                    case SDLK_RETURN:
                        return "RETURN";
                    case SDLK_SPACE:
                        return "SPACE";
                    case SDLK_p:
                        pause = !pause;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }
    return "";
}

void Engine::render_next_piece() {
    for (auto &b: game.next_piece.blocks) {
        render_block(b.col + COLS / 2 + 2, b.row, b.color);
    }
}

void Engine::render_text_box(const char *text) {
    SDL_SetRenderDrawColor(renderer, 135, 135, 135, 0);
    auto w = COLS * SCALE;
    auto h = 4 * SCALE;
    auto x = WIDTH / 2 - COLS * SCALE / 2;
    auto y = HEIGHT / 2 - h - h / 2;

    auto rect = SDL_Rect{x - 6, y - 6, w + 12, h + 12};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 0);

    rect = SDL_Rect{x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
    render_text(text, x + w / 2, y, font);
}

void Engine::render_text(const char *text, int x, int y, TTF_Font *fnt) {
    auto s_text = TTF_RenderUTF8_Blended(fnt, text, SDL_Color{135, 135, 135, 0});
    if (s_text == nullptr) {
        // TODO: throw exception
    }
    auto texture = SDL_CreateTextureFromSurface(renderer, s_text);
    if (texture == nullptr) {
        // TODO: throw exception
    }

    auto rect = SDL_Rect{x - s_text->w / 2, y + s_text->h / 2 - SCALE / 10, s_text->w, s_text->h};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(s_text);
    SDL_DestroyTexture(texture);
}

void Engine::render_canvas_block(int x, int y) {
    SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255);

    auto rect = SDL_Rect{x * SCALE + X_OFFSET, y * SCALE + Y_OFFSET, SCALE, SCALE};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 150);
    SDL_RenderDrawRect(renderer, &rect);
}

void Engine::render_block(int x, int y, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    auto rect = SDL_Rect{x * SCALE + X_OFFSET, y * SCALE + Y_OFFSET, SCALE, SCALE};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 150);
    SDL_RenderDrawRect(renderer, &rect);
}

void Engine::render_stats() {
    auto h = 4 * SCALE;
    auto x = WIDTH / 2 - COLS * SCALE / 2;
    auto y = HEIGHT / 2 - 3 * h;

    render_text("Level:", x - 3 * SCALE, y, font_small);
    auto level_s = std::to_string(game.level);
    render_text(level_s.c_str(), x - 3 * SCALE, y + SCALE + SCALE / 2, font_small);

    render_text("Score:", x - 3 * SCALE, y + h, font_small);
    auto score_s = std::to_string(game.score);
    render_text(score_s.c_str(), x - 3 * SCALE, y + SCALE + SCALE / 2 + h, font_small);
}

void Engine::render() {
    SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
    auto rect = SDL_Rect{X_OFFSET - 6, Y_OFFSET - 6, COLS * SCALE + 12, ROWS * SCALE + 12};
    SDL_RenderFillRect(renderer, &rect);

    auto y = 0;
    for (auto &row: game.board.canvas) {
        auto x = 0;
        for (auto b: row) {
            if (b.dead) {
                render_canvas_block(x, y);
            } else {
                render_block(b.col, b.row, b.color);
            }
            ++x;
        }
        ++y;
    }

    render_next_piece();
    render_stats();

    if (pause) render_text_box("Pause");
    if (game.game_over) render_text_box("Game Over");

    SDL_RenderPresent(renderer);
}