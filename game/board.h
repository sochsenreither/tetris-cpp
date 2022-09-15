#pragma once

#include <array>
#include "piece.h"
#include "block.h"

struct Board {
private:
    std::array<std::array<Block, COLS>, ROWS> canvas;

public:
    void draw_piece(const Piece &p);

    void remove_piece(const Piece &p);

    void move_canvas_down(int index);

    bool collision(const Piece &p);

    bool can_clear_lines(int index);

    int clear_lines();

    void debug_print();
};