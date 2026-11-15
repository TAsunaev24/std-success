#ifndef RENDERER_H_
#define RENDERER_H_

#include "field.h"
#include "raylib.h"

class Renderer
{
	private:
		Field _f;
		int _maxFowRadius;
		int _fowRadius;
		int _frameNumber;

		int _getCellSize();
		bool _isInFowRadius(Vector2 cell);
		bool _hasReachedEnd();
		Vector2 _getFieldSides();
		Vector2 _getTopLeftPoint();
		Vector2 _getCellTopLeft(Vector2 cell);
		void _checkChangeRadius(); // animation
		int _getMaxFowRadius();

		coordinate _playerPos;
	public:
		bool shouldClose;
		void tryMove(enum generationDirection direction);
		Renderer(int height, int width);
		Renderer(int height, int width, int fowRadius);
		void drawFrame();
};

#endif
