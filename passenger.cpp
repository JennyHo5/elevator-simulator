#include "passenger.h"

Passenger::Passenger(int id, Floor* floor):
    passengerID(id),
    currentFloor(floor),
    currentElevator(nullptr),
    beOutside(true),
    beInside(false)
{
}

void Passenger::print() const {
    qInfo("Passenger %d on floor %d", passengerID, currentFloor->getFloorNumber());
}
