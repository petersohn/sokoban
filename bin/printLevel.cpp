#include "Dumper/DumperFunctions.hpp"

#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
        return 1;
    }

    auto data(sokoban::loadStatusFromFile(argv[1]));
    sokoban::Status& status = data.second;
    sokoban::dumpStatus(std::cout, status);
}

