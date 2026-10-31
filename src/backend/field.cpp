// All bool functions return 0 (false) on success and 1 (true) on error

#include <vector>
#include <algorithm>
#include "field.h"
#include "utils.h"

using namespace std;

bool operator==(const coordinate& a, const coordinate& b)
{
	return (a.x == b.x) && (a.y == b.y);
}

bool operator!=(const coordinate& a, const coordinate& b)
{
	return (a.x != b.x) || (a.y != b.y);
}

// check if has got into position which has no way to continue
bool Field::_hitEnd(coordinate cell)
{
	return (_checkCell(applyDirection(cell, UP), cell) && _checkCell(applyDirection(cell, LEFT), cell) && _checkCell(applyDirection(cell, RIGHT), cell) && _checkCell(applyDirection(cell, DOWN), cell));
}

void Field::_generateField()
{
	vector<coordinate> queue;
	coordinate
		start = { 0, 0 },
		exchange = start,
		end = { _h - 1, _w - 1 };
	setCell(start, EMPTY);

	// main path
	while (start != end && !_hitEnd(start))
	{
		if (getAction() == SPLIT)
			if (find(queue.begin(), queue.end(), start) == queue.end())
				queue.push_back(start);

		if (_getPossibleDirections(start).size()==1)
			exchange = applyDirection(start, _getPossibleDirections(start)[0]);
		else
			exchange = applyDirection(start, getDirection(start, end));
		if ((getCell(exchange) != WALL) || _checkCell(exchange, start)) continue;
		setCell(exchange, EMPTY);
		start = exchange;
	}

	enum generationAction action;
	// handle queue splits
	// TODO? add splits to splits
	while (queue.size())
	{
		start = *(queue.end()-1);
		queue.pop_back();
		while (!_hitEnd(start))
		{
			/*if (_getPossibleDirections(start).size()==1)
				exchange = applyDirection(start, _getPossibleDirections(start)[0]);
			else*/
				exchange = applyDirection(start, getDirection(start, start));
			if ((getCell(exchange) != WALL) || _checkCell(exchange, start)) continue;
			setCell(start, EMPTY);
			start = exchange;
		}
	}
}

// return true if oob
bool Field::_checkOOB(coordinate cell)
{
	return cell.x<0 || cell.y<0 || cell.x>=_w || cell.y>=_h;
}

// returns true if the call cant be used
bool Field::_checkCell(coordinate cell, coordinate prevCell)
{
	if ((!_checkOOB(applyDirection(cell, UP))) && ((applyDirection(cell, UP) != prevCell) && (getCell(applyDirection(cell, UP)) == EMPTY))) return true; // UP
	if ((!_checkOOB(applyDirection(cell, LEFT))) && ((applyDirection(cell, LEFT) != prevCell) && (getCell(applyDirection(cell, LEFT)) == EMPTY))) return true; // LEFT
	if ((!_checkOOB(applyDirection(cell, RIGHT))) && ((applyDirection(cell, RIGHT) != prevCell) && (getCell(applyDirection(cell, RIGHT)) == EMPTY))) return true; // RIGHT
	if ((!_checkOOB(applyDirection(cell, DOWN))) && ((applyDirection(cell, DOWN) != prevCell) && (getCell(applyDirection(cell, DOWN)) == EMPTY))) return true; // DOWN
	return _checkOOB(cell) || (getCell(cell)==EMPTY);
}

vector<enum generationDirection> Field::_getPossibleDirections(coordinate cell)
{
	vector<enum generationDirection> ret;
	if (!_checkCell(applyDirection(cell, UP), cell)) ret.push_back(UP);
	if (!_checkCell(applyDirection(cell, LEFT), cell)) ret.push_back(LEFT);
	if (!_checkCell(applyDirection(cell, RIGHT), cell)) ret.push_back(RIGHT);
	if (!_checkCell(applyDirection(cell, DOWN), cell)) ret.push_back(DOWN);
	return ret;
}

enum cellType Field::getCell(coordinate cell)
{
	if (_checkOOB(cell)) return WALL;
	return _f[cell.y][cell.x];
}

enum cellType Field::getCell(int y, int x)
{
	return getCell({y, x});
}

bool Field::setCell(coordinate cell, enum cellType val)
{
	if (_checkOOB(cell)) return 1;
	_f[cell.y][cell.x] = val;
	return 0;
}

Field::Field(int height, int width)
{
	_h = height;
	_w = width;

	// alloc
	_f = new enum cellType*[_h];
	for (int i = 0; i < _h; ++i)
	{
		_f[i] = new enum cellType[_w];
		for (int j = 0; j<_w; ++j)
			_f[i][j] = WALL;
	}

	_generateField();
}

Field::~Field()
{
	if (!_f) return;
	for (int i = 0; i < _h; ++i)
		delete[] _f[i];
	delete[] _f;
}
