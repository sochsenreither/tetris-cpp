#include "board.h"
#include <iostream>

void Board::draw_piece(const Piece &p) {
    for (auto b: p.blocks) {
        canvas[b.row][b.col] = b;
    }
}

void Board::remove_piece(const Piece &p) {
    for (auto b: p.blocks) {
        canvas[b.row][b.col] = Block{b.row, b.col, b.color, false};
    }
}

bool Board::collision(const Piece &p) {
    return std::any_of(
            p.blocks.begin(),
            p.blocks.end(),
            [this](const Block &b) {
                if (canvas[b.row][b.col].active) return true;
                if (b.row < 0 || b.row > ROWS - 1) return true;
                if (b.col < 0 || b.col > COLS - 1) return true;
                return false;
            });
}

void Board::debug_print() {
    for (auto c: canvas) {
        for (auto b: c) {
            if (b.active) {
                std::cout << "o ";
            } else {
                std::cout << "_ ";
            }
        }
        std::cout << "\n";
    }
}

void Board::move_canvas_down(int index) {
    for (auto i = index; i > 0; --i) {
        for (auto j = 0; j < canvas[i].size(); ++j) {
            if (canvas[i - 1][j].active) {
                canvas[i][j] = canvas[i - 1][j];
                canvas[i][j].row += 1;
                canvas[i - 1][j].active = false;
            } else {
                canvas[i][j].active = false;
            }
        }
    }
}

bool Board::can_clear_lines(int index) {
    return std::all_of(
            canvas[index].begin(),
            canvas[index].end(),
            [](const Block &b) {
                if (b.active) return true;
                else return false;
            });
}

int Board::clear_lines() {
    auto count = 0;
    for (auto i = ROWS; i > 0; --i) {
        if (this->can_clear_lines(i)) {
            count++;
            this->move_canvas_down(i);
        }
    }
    return count;
}