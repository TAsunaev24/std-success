#ifndef UTILS_H_
#define UTILS_H_

#include "field.h"

enum fogPreset
{
	NONE,
	EASY,
	MEDIUM,
	HARD,
	VERY_HARD
};

coordinate applyDirection(coordinate current, enum generationDirection direction);
enum generationDirection getDirection(coordinate loc, coordinate end);
enum generationAction getAction();
unsigned int getRandom();
int getFogRadius(int size, enum fogPreset preset);
int manhattanDistance(coordinate first, coordinate second);

#endif
