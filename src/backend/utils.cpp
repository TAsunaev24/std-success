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
	return random() % 4;
}

// first run: foundWay = false, possible options: continue, split
// second run: foundWay = true, possible options: continue, stop, split		FOR NOW
enum generationAction getAction(bool foundWay)
{
	return random() % (2 + foundWay);
}

unsigned int random()
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	srand(ts.nsec ^ ts.sec);
	return rand();
}