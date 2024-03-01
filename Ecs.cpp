#include "Ecs.h"

ECS::ECS()
{
}

void ECS::addPassenger(Passenger * p) {
    passengers.push_back(p);
}

void ECS::addElevator(Elevator * e) {
    elevators.push_back(e);
}

void ECS::addFloorRequest(const Floor* f, Direction d) {
    FloorRequest fr = FloorRequest{f, d};
    floorRequests.push_back(fr);
}
