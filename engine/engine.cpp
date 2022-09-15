#include "engine.h"
#include <iostream>

Engine::Engine() {
    window = SDL_CreateWindow("tetris",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width,
                              height,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        // TODO: throw exception
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        // TODO: throw exception
    }
    if (TTF_Init() == -1) {
        // TODO: throw exception
    }

    font = TTF_OpenFont("font/SourceSansPro-Regular.otf", 48);
    if (font == nullptr) {
        // TODO: throw exception
    }
    font_small = TTF_OpenFont("font/SourceSansPro-Regular.otf", 32);
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
        std::cout << direction << '\n';

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

        //render();
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