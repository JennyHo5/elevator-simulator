#ifndef PASSENGER_H
#define PASSENGER_H

#include "floor.h"
#include "elevator.h"

class Passenger
{
public:
    Passenger(int id, Floor* currentFloor);
    void pressDirection(Direction);
    bool isOutside() const {return beOutside;}
    bool isInside() const {return beInside;}

private:
    int passengerID;
    Floor* currentFloor;
    Elevator* currentElevator;
    bool beOutside;
    bool beInside;
};

#endif // PASSENGER_H
