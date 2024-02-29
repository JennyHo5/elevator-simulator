#include "passenger.h"

Passenger::Passenger(int id, Floor* floor):
    passengerID(id),
    currentFloor(floor),
    currentElevator(nullptr),
    beOutside(true),
    beInside(false)
{
}
