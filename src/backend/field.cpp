// All bool functions return 0 (false) on success and 1 (true) on error

#include <vector>
#include <iostream> // DEBUG
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
bool Field::_hitEnd(coordinate loc)
{
	return (_checkCell({ loc.y - 1, loc.x }, loc) && _checkCell({ loc.y, loc.x - 1}, loc) && _checkCell({ loc.y, loc.x + 1}, loc) && _checkCell({ loc.y + 1, loc.x }, loc));
}

void Field::_dumpField()
{
	for (int i = 0; i<_h; ++i)
	{
		for (int j = 0; j<_w; ++j)
			cout << (_f[i][j]==WALL?'O':' ');
		cout << endl;
	}
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

		exchange = applyDirection(start, getDirection(start, end));
		if ((getCell(exchange) != WALL) || _checkCell(exchange, start)) continue;
		setCell(exchange, EMPTY);
		start = exchange;
	}

	cout << queue.size() << endl;

	enum generationAction action;
	// handle queue splits
	// TODO? add splits to splits
	while (queue.size())
	{
		start = *(queue.end()-1);
		queue.pop_back();
		while (!_hitEnd(start))
		{
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
	if ((!_checkOOB({ cell.y - 1, cell.x })) && (((coordinate){ cell.y - 1, cell.x } != prevCell) && (getCell({ cell.y - 1, cell.x }) == EMPTY))) return true; // UP
	if ((!_checkOOB({ cell.y, cell.x - 1 })) && (((coordinate){ cell.y, cell.x - 1 } != prevCell) && (getCell({ cell.y, cell.x - 1 }) == EMPTY))) return true; // LEFT
	if ((!_checkOOB({ cell.y, cell.x + 1 })) &&  (((coordinate){ cell.y, cell.x + 1 } != prevCell) && (getCell({ cell.y, cell.x + 1 }) == EMPTY))) return true; // RIGHT
	if ((!_checkOOB({ cell.y + 1, cell.x })) && (((coordinate){ cell.y + 1, cell.x } != prevCell) && (getCell({ cell.y + 1, cell.x }) == EMPTY))) return true; // DOWN
	return _checkOOB(cell) || (getCell(cell)==EMPTY);
}

enum cellType Field::getCell(coordinate loc)
{
	if (_checkOOB(loc)) return WALL;
	return _f[loc.y][loc.x];
}

enum cellType Field::getCell(int y, int x)
{
	return getCell({y, x});
}

bool Field::setCell(coordinate loc, enum cellType val)
{
	if (_checkOOB(loc)) return 1;
	_f[loc.y][loc.x] = val;
	return 0;
}

Field::Field(int height, int width)
{
	_h = height;
	_w = width;

	// alloc
	_f = new enum cellType*[_h];
	for (int i = 0; i < _h; ++i)
		_f[i] = new enum cellType[_w];

	_generateField();
}

Field::~Field()
{
	if (!_f) return;
	for (int i = 0; i < _h; ++i)
		delete[] _f[i];
	delete[] _f;
}
