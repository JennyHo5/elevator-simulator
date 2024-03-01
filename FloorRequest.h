#ifndef FLOORREQUEST_H
#define FLOORREQUEST_H

#include "Direction.h"
#include "Floor.h"

struct FloorRequest {
    const Floor* floor;
    Direction direction;
};

#endif // FLOORREQUEST_H
