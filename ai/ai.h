#pragma once

#include "../game/piece.h"
#include "../game/board.h"

class TetrisPlayer {
private:
    static double calculate_weight(Board &board, const Piece &piece);

    static Piece move_down(Board &board, Piece piece);

    static Piece move_left(Board &board, Piece piece);

    static int aggregate_height(const Board &board);

    static int complete_lines(Board &board);

    static int holes(const Board &board);

    static int bumpiness(const Board &board);

    static int column_height(const Board &board, int col);

public:
    static Piece next_move(Board board, const Piece &piece);
};