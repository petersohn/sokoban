#include "Status/StatusCreator.hpp"
#include "Status/Status.hpp"
#include "Exception.hpp"
#include <fstream>
#include <sstream>


std::pair<std::unique_ptr<Table>, Status>
createStatus(int width, int height, const std::vector<std::string>& lines)
{
	std::unique_ptr<Table> table{new Table(width, height)};
	State state;
	bool startPosOK = false, destinationOK = false;
	int stoneNum = 0;
	Point p;
	for (const std::string& line: lines)
	{
		for (p.x = 0; p.x < static_cast<int>(line.length()) && p.x < width; p.x++)
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
				++stoneNum;
				break;
			default:
				table->wall(p, true);
			}
		}
		if (++p.y >= height)
			break;
	}
	if (stoneNum == 0 || !startPosOK || !destinationOK)
		throw SokobanException();
	Status result(*table, state);
	result.currentPos(table->startingPoint());
	return {std::move(table), result};
}

std::pair<std::unique_ptr<Table>, Status>
loadStatusFromFile(const char *filename) {
	using namespace std;
	ifstream file(filename, ifstream::in);
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
	ss >> height >> width;
	string line;
	getline(ss, line); // dummy
	std::vector<std::string> lines;
	lines.reserve(height);
	while (ss.good())
	{
		getline(ss, line);
		lines.push_back(std::move(line));
	}
	return createStatus(width, height, lines);
}


