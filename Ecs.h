#ifndef ECS_H
#define ECS_H

#include "passenger.h"
#include "FloorRequest.h"
#include <vector>
#include <QDebug>

class ECS
{
public:
    ECS();
    ~ECS();
    void addPassenger(Passenger*);
    Passenger* getPassengerById(int id) {return passengers[id - 1];}
    void addElevator(Elevator*);
    void addFloorRequest(const Floor*, Direction);

private:
    std::vector<Passenger*> passengers;
    std::vector<Elevator*> elevators;
    std::vector<FloorRequest> floorRequests;
};

#endif // ECS_H
