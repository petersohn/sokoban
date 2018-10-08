#include "Sokoban.hpp"

int main(int argc, const char* argv[]) {
    try {
        sokoban::Sokoban sokoban{argc, argv};
        return sokoban.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}
