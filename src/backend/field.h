#ifndef FIELD_H_
#define FIELD_H_

#include <vector>

// defines how random the search will be
// bigger value represents less randomness but yields higher field generation success chance (see tests/pathtest.cpp)
// the value of 0 yields around 50% success chance, 1 - ~90%, 2 - ~96% for 25x25 field
#define LINEARITY 1

using namespace std;

typedef struct {
	int y;
	int x;
} coordinate;

bool operator==(const coordinate&, const coordinate&);
bool operator!=(const coordinate&, const coordinate&);

enum cellType {
	WALL,
	EMPTY
};

enum generationDirection {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum generationAction {
	CONTINUE,
	SPLIT
};

typedef enum cellType** field;

class Field
{
private:
	int _h;
	int _w;
	field _f = NULL;
	void _generateField();
	bool _checkCell(coordinate cell, coordinate prevCell);
	bool _checkOOB(coordinate cell);
	bool _hitEnd(coordinate cell);
	vector<enum generationDirection> _getPossibleDirections(coordinate cell);
public:
	enum cellType getCell(coordinate cell);
	enum cellType getCell(int y, int x);
	bool setCell(coordinate cell, enum cellType val);
	Field(int height, int width);
	~Field();
};

#endif
