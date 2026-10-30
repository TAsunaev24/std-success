#include <ctime>
#include <cstdlib>
#include "field.h"
#include "utils.h"

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

enum generationDirection getDirection()
{
	return (enum generationDirection)(getRandom() % 4);
}

// first run: foundWay = false, possible options: continue, split
// second run: foundWay = true, possible options: continue, stop, split		FOR NOW
enum generationAction getAction(bool foundWay)
{
	return (enum generationAction)(getRandom() % (2 + foundWay));
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
