#include "ai.h"

const auto height_weight = 0.510066;
const auto lines_weight = 0.760666;
const auto holes_weight = 0.35663;
const auto bumpiness_weight = 0.184483;

int TetrisPlayer::column_height(const Board &board, int col) {
    auto count = 0;
    for (auto row: board.canvas) {
        if (row[col].dead) count++;
        else return ROWS - count;
    }
    return ROWS - count;
}

int TetrisPlayer::bumpiness(const Board &board) {
    auto count = 0;
    for (auto col = 0; col < COLS - 1; ++col) {
        auto diff = column_height(board, col) - column_height(board, col + 1);
        if (diff < 0) count += diff * -1;
        else count += diff;
    }
    return count;
}

int TetrisPlayer::holes(const Board &board) {
    auto count = 0;
    for (auto col = 0; col < COLS; ++col) {
        auto b = false;
        for (auto row = 0; row < ROWS; ++row) {
            if (!board.canvas[row][col].dead) b = true;
            else if (board.canvas[row][col].dead && b) ++count;
        }
    }
    return count;
}

int TetrisPlayer::complete_lines(Board &board) {
    auto count = 0;
    for (auto i = 0; i < ROWS; ++i) {
        if (board.can_clear_lines(i)) ++count;
    }
    return count;
}

int TetrisPlayer::aggregate_height(const Board &board) {
    auto count = 0;
    for (auto i = 0; i < COLS; ++i) {
        count += column_height(board, i);
    }
    return count;
}

// TODO: refactor? Duplicate function
Piece TetrisPlayer::move_left(Board &board, Piece piece) {
    auto collision = false;

    while (!collision) {
        auto p = piece.move_left();
        if (board.collision(p)) collision = true;
        else piece = p;
    }
    return piece;
}

Piece TetrisPlayer::move_down(Board &board, Piece piece) {
    auto collision = false;

    while (!collision) {
        auto p = piece.move_down();
        if (board.collision(p)) collision = true;
        else piece = p;
    }
    return piece;
}

double TetrisPlayer::calculate_weight(Board &board, const Piece &piece) {
    board.draw_piece(piece);
    auto score = -height_weight * aggregate_height(board) +
                 lines_weight * complete_lines(board) -
                 holes_weight * holes(board) -
                 bumpiness_weight * bumpiness(board);
    board.remove_piece(piece);
    return score;
}

Piece TetrisPlayer::next_move(Board board, const Piece &piece) {
    board.remove_piece(piece);

    auto best_score = 0.0;
    auto best = Piece{};

    for (auto rotation = 0; rotation < 4; ++rotation) {
        auto working_piece = piece;

        // Rotate piece
        for (auto i = 0; i < rotation; ++i) {
            working_piece = working_piece.rotate();
        }

        // Move piece to the left
        working_piece = move_left(board, working_piece);

        auto collision = false;

        while (!collision) {
            auto tmp_piece = working_piece;

            // Move piece down as far as possible
            tmp_piece = move_down(board, tmp_piece);

            // Calculate score
            auto score = calculate_weight(board, tmp_piece);

            if (score > best_score || best_score == 0.0) {
                best_score = score;
                best = working_piece;
            }

            auto p = working_piece.move_right();
            if (board.collision(p)) collision = true;
            else working_piece = p;
        }
    }

    board.draw_piece(best);
    return best;
}