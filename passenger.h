#ifndef PASSENGER_H
#define PASSENGER_H

#include "elevator.h"

class Passenger
{
public:
    Passenger(int id, Floor* currentFloor);
    void pressDirection(Direction);
    bool isOutside() const {return beOutside;}
    bool isInside() const {return beInside;}
    const Floor* getCurrentFloor() const {return currentFloor;}

    void print() const;

private:
    int passengerID;
    Floor* currentFloor;
    Elevator* currentElevator;
    bool beOutside;
    bool beInside;
};

#endif // PASSENGER_H
