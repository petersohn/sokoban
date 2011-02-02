#ifndef COMMON_H_
#define COMMON_H_

template<class T> class Array;
class Point;

class SokobanException:public std::exception
{
	virtual const char* what() const throw()
	{
		return "Sokoban exception";
	}
};

struct MinMax {
	int minX, maxX, minY, maxY;
};

enum FieldType { ftWall, ftFloor, ftStone };

void floodFill(const Array<FieldType> &table, const Point &p0, Array<bool> &result,
			std::deque<int> *border = NULL, MinMax *minmax = NULL);

#endif /* COMMON_H_ */
