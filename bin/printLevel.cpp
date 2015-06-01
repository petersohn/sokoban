#include "Dumper/DumperFunctions.hpp"

#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
        return 1;
    }

    auto data(loadStatusFromFile(argv[1]));
    Status& status = data.second;
    dumpStatus(std::cout, status);
}

