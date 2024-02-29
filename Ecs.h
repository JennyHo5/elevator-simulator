#ifndef ECS_H
#define ECS_H

#include "passenger.h"
#include <vector>

class ECS
{
public:
    ECS();
    void addPassenger(Passenger*);
    Passenger* getPassengerById(int id) {return passengers[id - 1];}
    void addElevator(Elevator*);

private:
    std::vector<Passenger*> passengers;
    std::vector<Elevator*> elevators;
};

#endif // ECS_H
