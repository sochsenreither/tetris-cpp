#pragma once

#include <string>
#include <vector>
#include <array>

#include "block.h"

struct Piece {
    std::string type;
    std::array<Block, 4> blocks;

    Piece move_down();

    Piece move_left();

    Piece move_right();

    Piece move(int x, int y);

    Piece rotate();

    void set_inactive();

    void debug_print();

    bool is_active();
};

struct PieceFactory {
private:
    std::vector<Piece> pieces;

    void init();

    static Piece i_piece();

    static Piece j_piece();

    static Piece l_piece();

    static Piece o_piece();

    static Piece s_piece();

    static Piece z_piece();

    static Piece t_piece();

public:
    PieceFactory();

    Piece random_piece();
};