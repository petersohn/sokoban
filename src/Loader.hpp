#ifndef LOADER_HPP
#define LOADER_HPP

#include "ArchiveFwd.hpp"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <fstream>
#include <string>

namespace sokoban {

class Loader {
public:
    explicit Loader(const std::string& filename):
            stream{filename, std::ios::in},
            archive{stream}
    {
    }

    InputArchive& get()
    {
        return archive;
    }
private:
    std::ifstream stream;
    InputArchive archive;
};

} // namespace sokoban

#endif // LOADER_HPP
