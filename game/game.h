#pragma once

#include <string>

#include "board.h"
#include "piece.h"

struct Game {
    Board board{};
    Piece active_piece{};
    Piece next_piece{};
    PieceFactory pf{};
    int score{0};
    int level{0};
    int cleared_lines{0};
    bool game_over{false};

    bool step(const std::string &direction, bool tick);

    void spawn_piece();

    void handle_dropped_piece();
};