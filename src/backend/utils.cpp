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

enum generationDirection getDirection(coordinate loc, coordinate end)
{
	vector<enum generationDirection> possibleDirections;
	if (loc==end) // TODO? add some bias for moving away from the center
	{
		for (int i = 0; i<4; ++i)
			possibleDirections.push_back((enum generationDirection)i);
	}
	else
	{
		if (loc.x<end.x)
		{
			possibleDirections.push_back(RIGHT);
			if (loc.x<end.x/2)
			{
				possibleDirections.push_back(UP);
				possibleDirections.push_back(RIGHT);
			}
		}
		if (loc.y<end.y)
		{
			possibleDirections.push_back(DOWN);
			if (loc.y<end.y/2)
			{
				possibleDirections.push_back(LEFT);
				possibleDirections.push_back(DOWN);
			}
		}
	}

	return possibleDirections[getRandom()%possibleDirections.size()];
}

// current ratio is 2:1 fo continue:split
enum generationAction getAction()
{
	return ((getRandom()%3==0) ? SPLIT : CONTINUE);
}

unsigned int getRandom()
{
	static unsigned int mask = 0;
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	srand(ts.tv_nsec ^ ts.tv_sec);
	mask ^= (ts.tv_nsec + ts.tv_sec);
	return rand()+mask;
}
