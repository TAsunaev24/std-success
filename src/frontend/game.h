#ifndef MAZE_GAME_H_
#define MAZE_GAME_H_

#include <memory>
#include <vector>

#include "field.h"
#include "raylib.h"

class MazeGame
{
public:
	static constexpr float HUD_HEIGHT = 76.0f;
	static constexpr float FOOTER_HEIGHT = 44.0f;

	explicit MazeGame(int size);
	void restart();
	void update(float dt);
	void draw() const;

	bool won() const { return _won; }
	int size() const { return _size; }
	int moves() const { return _moves; }
	float elapsed() const { return _elapsed; }

private:
	void generate();
	bool hasPath() const;
	bool tryMove(int dx, int dy);

	int _size;
	std::unique_ptr<Field> _field;
	coordinate _player{0, 0};
	coordinate _exit{0, 0};
	std::vector<unsigned char> _visited;

	int _moves = 0;
	float _elapsed = 0.0f;
	bool _won = false;
	bool _started = false;

	int _holdX = 0;
	int _holdY = 0;
	float _repeatTimer = 0.0f;
	Vector2 _visualPlayer{0.0f, 0.0f};
};

#endif
