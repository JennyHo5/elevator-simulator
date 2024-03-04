#include "passenger.h"

Passenger::Passenger(int id, Floor* floor, QObject *parent):
    QObject(parent),
    passengerID(id),
    currentFloor(floor),
    currentElevator(nullptr),
    beOutside(true),
    beInside(false),
    beWaitingForElevator(false)
{
}

Passenger::~Passenger() {
}

void Passenger::pressDirection(Direction d) {
    beWaitingForElevator = true;
    if (d == Direction::UP)
        emit messageReceived("[Passenger " + QString::number(passengerID) + "] Presses UP button on floor " + QString::number(getCurrentFloor()->getFloorNumber()));
    else
        emit messageReceived("[Passenger " + QString::number(passengerID) + "] Presses DOWN button on floor " + QString::number(getCurrentFloor()->getFloorNumber()));
}

void Passenger::enterElevator(Elevator* e) {
    emit messageReceived("[Pessenger " + QString::number(passengerID) + "] Enters Elevator " + QString::number(e->getElevatorID()));
    beOutside = false;
    beInside = true;
    currentElevator = e;
    beWaitingForElevator = false;
}

void Passenger::pressFloorNumber(int floorNumber) {
    emit floorNumberPressed(floorNumber, currentElevator);
    requiredFloorNumber = floorNumber;
    emit messageReceived("[Passenger " + QString::number(passengerID) + "] Presses floor number " + QString::number(floorNumber));
}
