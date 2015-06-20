#ifndef LOADER_HPP
#define LOADER_HPP

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <fstream>
#include <string>

using InputArchive = boost::archive::binary_iarchive;
using OutputArchive = boost::archive::binary_oarchive;

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


#endif // LOADER_HPP
