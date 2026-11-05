#include "game.h"

#include <algorithm>
#include <cmath>
#include <queue>

#include "ui.h"

namespace
{
bool IsInside(int y, int x, int size)
{
	return y >= 0 && x >= 0 && y < size && x < size;
}
} // namespace

MazeGame::MazeGame(int size)
	: _size(std::max(2, size))
{
	generate();
}

void MazeGame::restart()
{
	generate();
}

bool MazeGame::hasPath() const
{
	if (!_field) return false;
	if (_field->getCell(0, 0) != EMPTY) return false;
	if (_field->getCell(_size - 1, _size - 1) != EMPTY) return false;

	std::vector<unsigned char> seen(static_cast<size_t>(_size) * _size, 0);
	std::queue<coordinate> open;
	open.push({0, 0});
	seen[0] = 1;

	const int dy[4] = {-1, 1, 0, 0};
	const int dx[4] = {0, 0, -1, 1};

	while (!open.empty())
	{
		coordinate current = open.front();
		open.pop();

		if (current.y == _size - 1 && current.x == _size - 1)
			return true;

		for (int i = 0; i < 4; ++i)
		{
			const int ny = current.y + dy[i];
			const int nx = current.x + dx[i];
			if (!IsInside(ny, nx, _size)) continue;
			if (seen[static_cast<size_t>(ny) * _size + nx]) continue;
			if (_field->getCell(ny, nx) != EMPTY) continue;

			seen[static_cast<size_t>(ny) * _size + nx] = 1;
			open.push({ny, nx});
		}
	}
	return false;
}

void MazeGame::generate()
{
	bool generated = false;
	constexpr int maxAttempts = 200;

	for (int attempt = 0; attempt < maxAttempts; ++attempt)
	{
		_field = std::make_unique<Field>(_size, _size);
		if (hasPath())
		{
			generated = true;
			break;
		}
	}

	// The generator is probabilistic. In the extremely unlikely case that no
	// connected maze was produced, carve a guaranteed path along the border.
	if (!generated && _field)
	{
		for (int x = 0; x < _size; ++x)
			_field->setCell({0, x}, EMPTY);
		for (int y = 0; y < _size; ++y)
			_field->setCell({y, _size - 1}, EMPTY);
	}

	_player = {0, 0};
	_exit = {_size - 1, _size - 1};
	_visited.assign(static_cast<size_t>(_size) * _size, 0);
	_visited[0] = 1;

	_moves = 0;
	_elapsed = 0.0f;
	_won = false;
	_started = false;
	_holdX = 0;
	_holdY = 0;
	_repeatTimer = 0.0f;
	_visualPlayer = {0.0f, 0.0f};
}

bool MazeGame::tryMove(int dx, int dy)
{
	if (!_field || _won) return false;

	const int ny = _player.y + dy;
	const int nx = _player.x + dx;
	if (!IsInside(ny, nx, _size)) return false;
	if (_field->getCell(ny, nx) == WALL) return false;

	_player = {ny, nx};
	_visited[static_cast<size_t>(ny) * _size + nx] = 1;
	++_moves;
	_started = true;

	if (_player == _exit)
		_won = true;

	return true;
}

void MazeGame::update(float dt)
{
	if (_won || !_field) return;

	dt = std::min(dt, 0.1f);
	if (_started)
		_elapsed += dt;

	int dx = 0;
	int dy = 0;
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) dy = -1;
	else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) dy = 1;
	else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) dx = -1;
	else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dx = 1;

	if (dx == 0 && dy == 0)
	{
		_holdX = 0;
		_holdY = 0;
		_repeatTimer = 0.0f;
	}
	else if (dx != _holdX || dy != _holdY)
	{
		_holdX = dx;
		_holdY = dy;
		_repeatTimer = 0.20f;
		tryMove(dx, dy);
	}
	else
	{
		_repeatTimer -= dt;
		if (_repeatTimer <= 0.0f)
		{
			tryMove(dx, dy);
			_repeatTimer = 0.085f;
		}
	}

	const float blend = 1.0f - std::exp(-20.0f * dt);
	_visualPlayer.x += (_player.x - _visualPlayer.x) * blend;
	_visualPlayer.y += (_player.y - _visualPlayer.y) * blend;
}

void MazeGame::draw() const
{
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();
	constexpr float sideMargin = 24.0f;

	const float availableWidth = static_cast<float>(screenWidth) - sideMargin * 2.0f;
	const float availableHeight = static_cast<float>(screenHeight) - HUD_HEIGHT - FOOTER_HEIGHT - 24.0f;
	float cell = std::floor(std::min(availableWidth / _size, availableHeight / _size));
	cell = std::max(cell, 2.0f);

	const float mazeWidth = cell * _size;
	const float mazeHeight = cell * _size;
	Vector2 origin{
		(screenWidth - mazeWidth) * 0.5f,
		HUD_HEIGHT + (screenHeight - HUD_HEIGHT - FOOTER_HEIGHT - mazeHeight) * 0.5f};

	Rectangle board{origin.x - 10.0f, origin.y - 10.0f, mazeWidth + 20.0f, mazeHeight + 20.0f};
	DrawRectangleRounded(board, 0.02f, 6, theme::Panel);

	DrawRectangleV(origin, {mazeWidth, mazeHeight}, theme::Floor);

	// A faint trail makes it easier to see which corridors were explored.
	if (cell >= 6.0f)
	{
		for (int y = 0; y < _size; ++y)
		{
			for (int x = 0; x < _size; ++x)
			{
				if (!_visited[static_cast<size_t>(y) * _size + x]) continue;
				if (y == _exit.y && x == _exit.x) continue;
				Rectangle dot{
					origin.x + x * cell + cell * 0.28f,
					origin.y + y * cell + cell * 0.28f,
					cell * 0.44f,
					cell * 0.44f};
				DrawRectangleV({dot.x, dot.y}, {dot.width, dot.height}, Fade(theme::Accent, 0.16f));
			}
		}
	}

	for (int y = 0; y < _size; ++y)
	{
		for (int x = 0; x < _size; ++x)
		{
			if (_field->getCell(y, x) != WALL) continue;

			Rectangle wall{
				origin.x + x * cell,
				origin.y + y * cell,
				cell,
				cell};
			DrawRectangleRec(wall, theme::Wall);

			if (cell >= 10.0f)
			{
				DrawLineEx({wall.x, wall.y + 1.0f}, {wall.x + wall.width, wall.y + 1.0f}, 1.0f,
					theme::WallHighlight);
				DrawLineEx({wall.x, wall.y + 1.0f}, {wall.x, wall.y + wall.height}, 1.0f,
					theme::WallHighlight);
				DrawLineEx({wall.x + wall.width, wall.y + 1.0f}, {wall.x + wall.width, wall.y + wall.height},
					1.0f, theme::WallShadow);
				DrawLineEx({wall.x, wall.y + wall.height}, {wall.x + wall.width, wall.y + wall.height},
					1.0f, theme::WallShadow);
			}
		}
	}

	// Entrance marker.
	Vector2 entrance{
		origin.x + 0.5f * cell,
		origin.y + 0.5f * cell};
	DrawCircleV(entrance, std::max(2.0f, cell * 0.18f), Fade(theme::Success, 0.45f));

	// Exit marker.
	const float pulse = 0.5f + 0.5f * std::sin(static_cast<float>(GetTime()) * 3.2f);
	Vector2 exit{
		origin.x + (_exit.x + 0.5f) * cell,
		origin.y + (_exit.y + 0.5f) * cell};
	const float exitRadius = std::max(3.0f, cell * 0.24f);
	DrawCircleV(exit, exitRadius * (1.6f + pulse * 0.4f), Fade(theme::Gold, 0.17f));
	DrawPoly(exit, 4, exitRadius, 45.0f, theme::Gold);
	DrawCircleV(exit, std::max(1.0f, exitRadius * 0.3f), theme::BackgroundBottom);

	// Player.
	Vector2 player{
		origin.x + (_visualPlayer.x + 0.5f) * cell,
		origin.y + (_visualPlayer.y + 0.5f) * cell};
	const float playerRadius = std::max(3.0f, cell * 0.32f);
	DrawCircleV(player, playerRadius * 1.9f, Fade(theme::Accent, 0.16f));
	DrawCircleV({player.x, player.y + 2.0f}, playerRadius, Fade(BLACK, 0.35f));
	DrawCircleV(player, playerRadius, theme::AccentHover);
	DrawCircleV(player, playerRadius * 0.42f, WHITE);

	DrawRectangleRoundedLinesEx(board, 0.02f, 6, 1.5f, theme::PanelBorder);
}
