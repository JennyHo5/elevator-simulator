#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "floor.h"

class Elevator
{
public:
    Elevator(int id, Floor* floor);

private:
    int elevatorID;
    Floor* currentFloor;
};

#endif // ELEVATOR_H
