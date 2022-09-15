#include "piece.h"
#include "block.h"

#include <algorithm>
#include <iostream>
#include <random>

const SDL_Color mint = {72, 207, 173};
const SDL_Color bluejeans = {93, 156, 236};
const SDL_Color bittersweet = {252, 110, 81};
const SDL_Color sunflower = {255, 206, 84};
const SDL_Color grass = {160, 212, 104};
const SDL_Color lavender = {172, 146, 236};
const SDL_Color ruby = {216, 51, 74};

Piece Piece::move(int x, int y) {
    std::array<Block, 4> new_blocks;
    auto i = 0;
    for (auto b: blocks) {
        new_blocks.at(i++) = Block{b.row + x, b.col + y, b.color, b.active};
    }

    return Piece{type, new_blocks};
}

Piece Piece::move_down() {
    return move(1, 0);
}

Piece Piece::move_left() {
    return move(0, -1);
}

Piece Piece::move_right() {
    return move(0, 1);
}

Piece Piece::rotate() {
    if (type == "O") {
        return *this;
    }

    std::array<Block, 4> new_blocks;
    auto pivot = blocks.front();
    auto counter = 0;
    new_blocks.at(counter) = pivot;

    for (auto b: blocks) {
        if (counter == 0) {
            ++counter;
            continue;
        }

        auto d_row = pivot.row - b.row;
        auto d_col = pivot.col - b.col;

        new_blocks.at(counter++) = Block{
                pivot.row + (d_col * -1),
                pivot.col + d_row,
                b.color,
                b.active};
    }

    return Piece{type, new_blocks};
}

void Piece::set_inactive() {
    for (auto b: blocks) {
        b.active = false;
    }
}

void Piece::debug_print() {
    std::cout << "Type: " << type << '\n';
    for (auto b: blocks) {
        std::cout << "Row: " << b.row << " Col: " << b.col << " Active: " << b.active << '\n';
    }
}

bool Piece::is_active() {
    return std::for_each(
            blocks.begin(),
            blocks.end(),
            [](const Block &b) {
                if (b.active) return true;
                else return false;
            });
}

PieceFactory::PieceFactory() {
    pieces.reserve(7);
    init();
}

void PieceFactory::init() {
    pieces = std::vector<Piece>{i_piece(), j_piece(), l_piece(), o_piece(), s_piece(), t_piece(), z_piece()};
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(pieces), std::end(pieces), rng);
}

Piece PieceFactory::random_piece() {
    if (pieces.empty()) {
        init();
    }

    auto p = pieces.back();
    pieces.pop_back();

    for (auto &b: p.blocks) {
        b.col += COLS / 2 - 1;
    }

    return p;
}

Piece PieceFactory::i_piece() {
    return Piece{
            "I",
            std::array<Block, 4>{
                    Block{1, 1, mint, true},
                    Block{1, 2, mint, true},
                    Block{1, 0, mint, true},
                    Block{1, 3, mint, true},
            }};
}

Piece PieceFactory::l_piece() {
    return Piece{
            "L",
            std::array<Block, 4>{
                    Block{1, 1, bittersweet, true},
                    Block{1, 0, bittersweet, true},
                    Block{1, 2, bittersweet, true},
                    Block{0, 2, bittersweet, true},
            }};
}

Piece PieceFactory::o_piece() {
    return Piece{
            "O",
            std::array<Block, 4>{
                    Block{0, 0, sunflower, true},
                    Block{1, 1, sunflower, true},
                    Block{0, 1, sunflower, true},
                    Block{1, 0, sunflower, true},
            }
    };
}

Piece PieceFactory::s_piece() {
    return Piece{
            "S",
            std::array<Block, 4>{
                    Block{1, 1, grass, true},
                    Block{1, 0, grass, true},
                    Block{0, 1, grass, true},
                    Block{0, 2, grass, true},
            }
    };
}

Piece PieceFactory::j_piece() {
    return Piece{
            "J",
            std::array<Block, 4>{
                    Block{1, 1, bluejeans, true},
                    Block{1, 0, bluejeans, true},
                    Block{0, 0, bluejeans, true},
                    Block{1, 2, bluejeans, true},
            }
    };
}

Piece PieceFactory::t_piece() {
    return Piece{
            "T",
            std::array<Block, 4>{
                    Block{1, 1, lavender, true},
                    Block{1, 0, lavender, true},
                    Block{0, 1, lavender, true},
                    Block{1, 2, lavender, true},
            }
    };
}

Piece PieceFactory::z_piece() {
    return Piece{
            "Z",
            std::array<Block, 4>{
                    Block{1, 1, ruby, true},
                    Block{0, 0, ruby, true},
                    Block{0, 1, ruby, true},
                    Block{1, 2, ruby, true},
            }
    };
}