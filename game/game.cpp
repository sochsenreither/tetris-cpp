#include "game.h"

void Game::handle_dropped_piece() {
    board.draw_piece(active_piece);
    active_piece.set_inactive();
    auto count = board.clear_lines();

    int points[] = {0, 40, 100, 300, 1200};

    score += points[count] * (level + 1);
    cleared_lines += count;
}

void Game::spawn_piece() {
    if (active_piece.is_active()) return;
    if (!next_piece.is_active()) {
        next_piece = pf.random_piece();
    }
    if (board.collision(next_piece)) {
        game_over = true;
    }
    active_piece = next_piece;
    next_piece = pf.random_piece();
}

bool Game::step(const std::string &direction, bool tick) {
    if (cleared_lines >= 10) {
        level++;
        cleared_lines = 0;
    }

    Piece p;

    if (tick) {
        p = active_piece.move_down();
        if (board.collision(p)) {
            handle_dropped_piece();
            return true;
        }
        board.remove_piece(active_piece);
        active_piece = p;
    }

    if (direction == "DOWN") {
        p = active_piece.move_down();
        if (board.collision(p)) {
            handle_dropped_piece();
            return true;
        }
        score += 1;
    }
    if (direction == "UP") p = active_piece.rotate();
    if (direction == "LEFT") p = active_piece.move_left();
    if (direction == "RIGHT") p = active_piece.move_right();
    if (direction == "SPACE") {
        while (true) {
            p = active_piece.move_down();
            if (board.collision(p)) {
                handle_dropped_piece();
                return true;
            }
            board.remove_piece(active_piece);
            active_piece = p;
            score += 2;
        }
    }

    if (direction.empty()) {
        board.draw_piece(active_piece);
        return false;
    }

    if (!board.collision(p)) {
        board.remove_piece(active_piece);
        active_piece = p;
        board.draw_piece(active_piece);
    }
    return false;
}