#include <chrono>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "field.h"
#include "utils.h"

using namespace std;

coordinate applyDirection(coordinate current, enum generationDirection direction)
{
	switch (direction)
	{
		case UP:
		{
			--current.y;
			break;
		}
		case DOWN:
		{
			++current.y;
			break;
		}
		case LEFT:
		{
			--current.x;
			break;
		}
		case RIGHT:
		{
			++current.x;
			break;
		}
	}
	return current;
}

int getFogRadius(int size, enum fogPreset preset)
{
	switch (preset)
	{
		case NONE:
			return size*2-2;
		case EASY:
			return size*3/4;
		case MEDIUM:
			return size;
		case HARD:
			return size / 2;
		case VERY_HARD:
			return size / 4;
	}
}

int manhattanDistance(coordinate a, coordinate b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

enum generationDirection getDirection(coordinate loc, coordinate end)
{
	vector<enum generationDirection> possibleDirections;
	if (loc==end)
	{
		for (int i = 0; i<4; ++i)
			possibleDirections.push_back((enum generationDirection)i);

		// increase chance for moving away from the center of the screen
		if (loc.y<end.y/2)
			for (int i = 0; i<LINEARITY; ++i)
				possibleDirections.push_back(UP);
		else
			for (int i = 0; i<LINEARITY; ++i)
				possibleDirections.push_back(DOWN);

		if (loc.x<end.x/2)
			for (int i = 0; i<LINEARITY; ++i)
				possibleDirections.push_back(LEFT);
		else 
			for (int i = 0; i<LINEARITY; ++i)
				possibleDirections.push_back(RIGHT);
	}
	else
	{
		if (loc.x<end.x)
		{
			possibleDirections.push_back(RIGHT);
			if (loc.x<end.x-end.x/3)
			{
				possibleDirections.push_back(UP);
				for (int i = 0; i<LINEARITY; ++i)
					possibleDirections.push_back(RIGHT);
			}
		}
		if (loc.y<end.y)
		{
			possibleDirections.push_back(DOWN);
			if (loc.y<end.y-end.y/3)
			{
				possibleDirections.push_back(LEFT);
				for (int i = 0; i<LINEARITY; ++i)
					possibleDirections.push_back(DOWN);
			}
		}
	}

	return possibleDirections[getRandom()%possibleDirections.size()];
}

// current ratio is 2:1 fo continue:split
enum generationAction getAction()
{
	return ((getRandom()%3!=0) ? SPLIT : CONTINUE);
}

unsigned int getRandom()
{
	static unsigned int mask = 0;
	auto dt = chrono::high_resolution_clock::now();
	srand(chrono::nanoseconds(dt.time_since_epoch()).count() ^ time(NULL));
	mask ^= (chrono::nanoseconds(dt.time_since_epoch()).count() + time(NULL));
	return rand()+mask;
}
