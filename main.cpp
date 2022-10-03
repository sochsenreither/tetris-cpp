#include <iostream>

#include "engine/engine.h"

int main() {
    try {
        Engine{}.run();
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << '\n';
        return 1;
    }

    return 0;
}