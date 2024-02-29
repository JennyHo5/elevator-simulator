#ifndef PASSENGER_H
#define PASSENGER_H

#include "floor.h"

class Passenger
{
public:
    Passenger(int id);
    void pressDirection(Direction);

private:
    int passengerID;
};

#endif // PASSENGER_H
