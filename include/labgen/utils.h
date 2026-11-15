#ifndef UTILS_H_
#define UTILS_H_

#include "field.h"

coordinate applyDirection(coordinate current, enum generationDirection direction);
enum generationDirection getDirection(coordinate loc, coordinate end);
enum generationAction getAction();
unsigned int getRandom();

#endif
