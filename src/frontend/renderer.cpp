#include <cstdlib>
#include "../backend/field.h"
#include "../../raylib/src/raylib.h"

class Renderer
{
	private:
		Field _f;
		int _fowRadius;

		int _getCellSize();
		bool _isInFowRadius(Vector2 cell);
		Vector2 _getFieldSides();
		Vector2 _getTopLeftPoint();
		Vector2 _getCellTopLeft(Vector2 cell);
		int _getMaxFowRadius();

		coordinate _playerPos;
	public:
		bool shouldClose;
		void tryMove(enum generationDirection direction);
		Renderer(int height, int width);
		Renderer(int height, int width, int fowRadius);
		void drawFrame();
};


#define PADDING 30

int Renderer::_getCellSize()
{
	int renderH = GetRenderHeight(), renderW = GetRenderWidth();
	int wCellSize = (renderW - PADDING*2) / _f.getSize().x;
	int hCellSize = (renderH - PADDING*2) / _f.getSize().y;

	if (wCellSize*_f.getSize().y+PADDING >= renderH || hCellSize*_f.getSize().x+PADDING >= renderW) return min(wCellSize, hCellSize);
	else return max(wCellSize, hCellSize);
}

Vector2 Renderer::_getFieldSides()
{
	Vector2 size;
	size.x = _getCellSize() * _f.getSize().x;
	
	size.y = _getCellSize() * _f.getSize().y;
	return size;
}

Vector2 Renderer::_getTopLeftPoint()
{
	Vector2 point;
	point.x = (GetRenderWidth() - _getFieldSides().x) / 2;
	point.y = (GetRenderHeight() - _getFieldSides().y) / 2;
	return point;
}

Vector2 Renderer::_getCellTopLeft(Vector2 cell)
{
	Vector2 point;
	point.x = _getTopLeftPoint().x + _getCellSize() * cell.x;
	point.y = _getTopLeftPoint().y + _getCellSize() * cell.y;
	return point;
}

bool Renderer::_isInFowRadius(Vector2 cell)
{
	return (abs(cell.x - _playerPos.x) + abs(cell.y - _playerPos.y)) < _fowRadius;
}

int Renderer::_getMaxFowRadius()
{
	return _f.getSize().x + _f.getSize().y - 2;
}

void Renderer::tryMove(enum generationDirection direction)
{
	switch (direction)
	{
		case UP:
		{
			if (_f.getCell(_playerPos.y - 1, _playerPos.x) == EMPTY) --_playerPos.y;
			break;
		}
		case DOWN:
		{
			if (_f.getCell(_playerPos.y + 1, _playerPos.x) == EMPTY) ++_playerPos.y;
			break;
		}
		case LEFT:
		{
			if (_f.getCell(_playerPos.y, _playerPos.x - 1) == EMPTY) --_playerPos.x;
			break;
		}
		case RIGHT:
		{
			if (_f.getCell(_playerPos.y, _playerPos.x + 1) == EMPTY) ++_playerPos.x;
			break;
		}		
	}
}

Renderer::Renderer(int height, int width) : _f(height, width, true)
{
	_fowRadius = _getMaxFowRadius();
	shouldClose = false;
	_playerPos = {0, 0};
}

Renderer::Renderer(int height, int width, int fowRadius) : _f(height, width, true)
{
	_fowRadius = max(fowRadius, _getMaxFowRadius());
	shouldClose = false;
	_playerPos = {0, 0};
}

void Renderer::drawFrame()
{
	Vector2 sides = _getFieldSides();
	Vector2 tl = _getTopLeftPoint();
	DrawRectangleLines(tl.x, tl.y, sides.x, sides.y, WHITE);

	for (int i = 0; i<_f.getSize().y; ++i)
	{
		for (int j = 0; j<_f.getSize().x; ++j)
		{
			int cs = _getCellSize();
			Vector2 c;
			c.x = j;
			c.y = i;
			
			if (!_isInFowRadius(c)) continue;

			Vector2 cell = _getCellTopLeft(c);
			if (i == _playerPos.y && j == _playerPos.x) DrawCircle(cell.x+cs/2, cell.y+cs/2, cs/2, ORANGE);
			if (_f.getCell(i, j) == WALL) DrawRectangle(cell.x, cell.y, cs, cs, WHITE);
		}
	}
}
