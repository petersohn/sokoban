#include "Status/StatusCreator.hpp"

#include "Status/Status.hpp"
#include "Exception.hpp"
#include "floodFill.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace sokoban {

std::pair<std::unique_ptr<Table>, Status>
createStatus(const std::vector<std::string>& lines)
{
    if (lines.empty()) {
        throw SokobanException("No input");
    }
    std::size_t width = std::max_element(lines.begin(), lines.end(),
            [](const std::string& lhs, const std::string& rhs) {
                return lhs.size() < rhs.size();
            })->size();
    if (width == 0) {
        throw SokobanException("Empty input");
    }
    std::size_t height = lines.size();
    auto table = std::make_unique<Table>(width, height);
    State state;
    bool startPosOK = false, destinationOK = false;
    Point p;
    for (const std::string& line: lines)
    {
        for (p.x = 0; p.x < static_cast<int>(line.length())
                && p.x < static_cast<int>(width); p.x++)
        {
            switch (line[p.x])
            {
            case 'X':
            case 'x':
                table->destination(p);
                table->wall(p, false);
                destinationOK = true;
                break;
            case 'Y':
            case 'y':
                table->startingPoint(p);
                table->wall(p, false);
                startPosOK = true;
                break;
            case '.':
                table->wall(p, false);
                break;
            case 'o':
            case 'O':
                table->wall(p, false);
                state.addStone(p);
                break;
            default:
                table->wall(p, true);
            }
        }
        if (++p.y >= static_cast<int>(height))
            break;
    }

    Status status{*table, {}, table->destination()};
    Matrix<bool> reachable{table->width(), table->height(), false};
    floodFill(status, table->destination(), reachable);
    for (Point p : matrixRange(reachable)) {
        if (!reachable[p]) {
            table->wall(p, true);
            state.removeStone(p);
        }
    }
    if (table->wall(table->startingPoint())) {
        throw SokobanException("Starting point and destination not connected.");
    }

    table->trim();
    if (table->width() == 0 || table->height() == 0) {
        throw SokobanException("Empty table");
    }

    if (state.size() == 0) {
        throw SokobanException("No stones");
    }
    if (!startPosOK) {
        throw SokobanException("Starting position not given");
    }
    if (!destinationOK) {
        throw SokobanException("Destination not given");
    }
    Status result(*table, state, table->startingPoint());
    return {std::move(table), result};
}

std::pair<std::unique_ptr<Table>, Status> loadStatusFromStream(std::istream& file) {
    using namespace std;
    stringstream ss;
    while (file.good()) {
        string s;
        getline(file, s);
        if (s.empty())
            continue;
        if (s[0] == '#') {
            s.erase(s.begin());
            ss << s << endl;
        }
    }
    ss.seekg(0);
    size_t height, width;
    ss >> height >> width; // keep this for backwards compatibility
    string line;
    getline(ss, line); // dummy
    std::vector<std::string> lines;
    lines.reserve(height);
    while (ss.good())
    {
        getline(ss, line);
        lines.push_back(std::move(line));
    }
    return createStatus(lines);
}

std::pair<std::unique_ptr<Table>, Status>
loadStatusFromFile(const char *filename) {
    std::ifstream file(filename, std::ifstream::in);
    return loadStatusFromStream(file);
}

} // namespace sokoban
