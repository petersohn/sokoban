#include "Sokoban.hpp"

int main(int argc, const char* argv[]) {
    sokoban::Sokoban sokoban{argc, argv};
    return sokoban.run();
}
