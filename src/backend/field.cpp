#include <vector>
#include <algorithm>
#include <iostream> // DEBUG
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

void Field::_dumpField() // DEBUG
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
	while (start != end)
	{
		if (getAction(false) == SPLIT)
		{
			if (find(queue.begin(), queue.end(), start) == queue.end())
			{
				queue.push_back(start);
				continue;
			}
		}
		exchange = applyDirection(start, getDirection());
		if ((getCell(exchange) != WALL) || _checkCell(exchange, start)) continue;
		setCell(exchange, EMPTY);
		start = exchange;
	}

	enum generationAction action;
	// split paths
	while (queue.size())
	{
		start = *(queue.end()-1);
		queue.pop_back();
		while (1)
		{
			action = getAction(true);
			exchange = applyDirection(start, getDirection());
			if ((getCell(exchange) != WALL) || _checkCell(exchange, start)) break;
			if (action == STOP) break;
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

// TODO replace with true/false
bool Field::_checkCell(coordinate cell, coordinate prevCell)
{
	if ((!_checkOOB({ cell.y - 1, cell.x })) && (((coordinate){ cell.y - 1, cell.x } != prevCell) && (getCell({ cell.y - 1, cell.x }) == EMPTY))) return 1; // UP
	if ((!_checkOOB({ cell.y, cell.x - 1 })) && (((coordinate){ cell.y, cell.x - 1 } != prevCell) && (getCell({ cell.y, cell.x - 1 }) == EMPTY))) return 1; // LEFT
	if ((!_checkOOB({ cell.y, cell.x + 1 })) &&  (((coordinate){ cell.y, cell.x + 1 } != prevCell) && (getCell({ cell.y, cell.x + 1 }) == EMPTY))) return 1; // RIGHT
	if ((!_checkOOB({ cell.y + 1, cell.x })) && (((coordinate){ cell.y + 1, cell.x } != prevCell) && (getCell({ cell.y + 1, cell.x }) == EMPTY))) return 1; // DOWN
	return _checkOOB(cell);
}

enum cellType Field::getCell(coordinate loc)
{
	if (loc.y < 0 || loc.x < 0 || loc.y >= _h || loc.x >= _w) return WALL;
	return _f[loc.y][loc.x];
}

enum cellType Field::getCell(int y, int x)
{
	if (y < 0 || x < 0 || y >= _h || x >= _w) return WALL;
	return _f[y][x];
}

bool Field::setCell(coordinate loc, enum cellType val)
{
	if (loc.x>=_w || loc.y>=_h) return 1;
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
