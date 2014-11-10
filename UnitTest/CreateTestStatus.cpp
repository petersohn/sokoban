#include "CreateTestStatus.hpp"
#include "Exception.hpp"

Status createTestStatus(const Table& table, const std::vector<std::string>& lines)
{
	Status result{table};
	Point p;
	bool currentPosOK = false;
	for (const std::string& line: lines)
	{
		for (p.x = 0; p.x < static_cast<int>(line.length()) &&
				p.x < static_cast<int>(table.width()); p.x++)
		{
			switch (line[p.x])
			{
			case 'o':
			case 'O':
				result.addStone(p);
				break;
			case 'y':
			case 'Y':
				result.currentPos(p);
				currentPosOK = true;
			}
		}
		if (++p.y >= static_cast<int>(table.height()))
			break;
	}
	if (!currentPosOK)
		throw SokobanException("Current position not given");
	return result;
}

