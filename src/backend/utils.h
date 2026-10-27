#ifndef UTILS_H_
#define UTILS_H_

#include "field.h"

coordinate applyDirection(coordinate current, enum generationDirection direction);
enum generationDirection getDirection();
enum generationAction getAction(bool foundWay)
unsigned int random();

#endif