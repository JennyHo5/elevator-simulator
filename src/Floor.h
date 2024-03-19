#ifndef FLOOR_H
#define FLOOR_H

#include "Direction.h"
#include <QtDebug>

class Floor
{
public:
    Floor(int floorNumber);
    int getFloorNumber() const {return floorNumber;}

private:
    int floorNumber;
    bool requestUp;
    bool requestDown;
};

#endif // FLOOR_H
