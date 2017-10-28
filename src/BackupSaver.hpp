#ifndef BACKUPSAVER_HPP
#define BACKUPSAVER_HPP

#include <boost/filesystem.hpp>

#include <fstream>

namespace sokoban {

class BackupSaver {
public:
    BackupSaver(const std::string& filename):
            filename(filename), temporaryFilename(filename + "~"),
            stream(temporaryFilename, std::ios::out | std::ios::trunc)
    {
    }

    ~BackupSaver()
    {
        boost::filesystem::remove(filename);
        boost::filesystem::rename(temporaryFilename, filename);
    }

    std::ofstream& get() { return stream; }
private:
    std::string filename;
    std::string temporaryFilename;
    std::ofstream stream;
};

} // namespace sokoban

#endif // BACKUPSAVER_HPP
