#include "StatusCreator.h"
#include "Status.h"
#include <fstream>
#include <sstream>


Status createStatus(int width, int height, const std::vector<std::string>& lines)
{
	Table t(width, height);
	State st;
	Point startPos;
	bool startPosOK = false, destinationOK = false;
	int stoneNum = 0;
	Point p;
	for (const std::string& line: lines)
	{
		for (p.x = 0; p.x < line.length() && p.x < width; p.x++)
		{
			switch (line[p.x])
			{
			case 'X':
			case 'x':
				t.destination(p);
				t.wall(p, false);
				destinationOK = true;
				break;
			case 'Y':
			case 'y':
				startPos = p;
				t.wall(p, false);
				startPosOK = true;
				break;
			case '.':
				t.wall(p, false);
				break;
			case 'o':
			case 'O':
				t.wall(p, false);
				st.addStone(p);
				++stoneNum;
				break;
			default:
				t.wall(p, true);
			}
		}
		if (++p.y >= height)
			break;
	}
	if (stoneNum == 0 || !startPosOK || !destinationOK)
		throw SokobanException();
	Status result(FixedTable::Ptr(new FixedTable(t)), st);
	result.currentPos(startPos);
	return result;
}


Status loadStatusFromFile(const char *filename) {
	using namespace std;
	ifstream file(filename, ifstream::in);
	stringstream ss;
	while (file.good()) {
		string s;
		getline(file, s);
		if (s.size() == 0)
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


