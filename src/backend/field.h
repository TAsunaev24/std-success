#ifndef FIELD_H_
#define FIELD_H_

typedef struct {
	int y;
	int x;
} coordinate;

enum cellType {
	WALL,
	// COIN,  ?
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
	SPLIT,
	STOP
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
	void _dumpField(); // DEBUG
public:
	enum cellType getCell(coordinate loc);
	enum cellType getCell(int y, int x);
	bool setCell(coordinate loc, enum cellType val);
	Field(int height, int width);
	~Field();
};

#endif
