#include <cstdint>
#include <vector>
#include "field.h"

void Field::_generateField()
{
	vector<coordinate> queue;
	coordinate
		start = { 0, 0 },
		exchange = start,
		end = { _h - 1, _w - 1 };

	// main path
	while (start != end)
	{
		if (getAction(false) == SPLIT)
		{
			queue.push_back(start);
			break;
		}
		exchange = applyDirection(start, getDirection());
		if ((getCell(exchange) != WALL) || checkCell(exchange, start)) break;
		setCell(exchange, EMPTY);
		start = exchange;
	}

	enum generationAction action;
	// split paths
	while (queue.size())
	{
		start = queue.pop_back();
		while (1)
		{
			action = getAction(true);
			exchange = applyDirection(start, getDirection());
			if ((getCell(exchange) != WALL) || checkCell(exchange, start)) break;
			if (action == WALL) break;
			setCell(start, EMPTY);
			start = exchange;
		}
	}
}

bool Field::_checkCell(coordinate cell, coordinate prevCell)
{
	if (({ cell.y - 1, cell.x } != prevCell) && (getCell({ cell.y - 1, cell.x }) == EMPTY)) return 1; // UP
	if (({ cell.y, cell.x - 1 } != prevCell) && (getCell({ cell.y, cell.x - 1 }) == EMPTY)) return 1; // LEFT
	if (({ cell.y, cell.x + 1 } != prevCell) && (getCell({ cell.y, cell.x + 1 }) == EMPTY)) return 1; // RIGHT
	if (({ cell.y + 1, cell.x } != prevCell) && (getCell({ cell.y + 1, cell.x }) == EMPTY)) return 1; // DOWN
	return 0;
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