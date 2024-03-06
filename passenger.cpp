#include "passenger.h"

Passenger::Passenger(int id, Floor* floor, QObject *parent):
    QObject(parent),
    passengerID(id),
    currentFloor(floor),
    currentElevator(nullptr),
    beOutside(true),
    beInside(false),
    beWaitingForElevator(false),
    requiredFloorNumber(0)
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

void Passenger::pressHelp() {
    emit helpButtonPressed(currentElevator);
    emit messageReceived("[Passenger " + QString::number(passengerID) + "] Presses HELP button in elevator " + QString::number(getCurrentElevator()->getElevatorID()));
}

void Passenger::pressFloorNumber(int floorNumber) {
    emit floorNumberPressed(floorNumber, currentElevator);
    requiredFloorNumber = floorNumber;
    emit messageReceived("[Passenger " + QString::number(passengerID) + "] Presses floor number " + QString::number(floorNumber));
}

void Passenger::enterElevator(Elevator* e) {
    emit messageReceived("[Pessenger " + QString::number(passengerID) + "] Enters Elevator " + QString::number(e->getElevatorID()) + " on Floor " + QString::number(currentFloor->getFloorNumber()));
    beOutside = false;
    beInside = true;
    currentElevator = e;
    beWaitingForElevator = false;
    connect(e, &Elevator::currentFloorChanged, this, &Passenger::updateCurrentFloor);
}

void Passenger::exitElevator() {
    if (currentElevator != nullptr)
        emit messageReceived("[Pessenger " + QString::number(passengerID) + "] Exit Elevator " + QString::number(currentElevator->getElevatorID()) + " on Floor " + QString::number(currentFloor->getFloorNumber()));
    beOutside = true;
    beInside = false;
    disconnect(currentElevator, &Elevator::currentFloorChanged, this, &Passenger::updateCurrentFloor);
    currentElevator = nullptr;
    requiredFloorNumber = 0;
}

void Passenger::updateCurrentFloor(Floor* newFloor) {
    currentFloor = newFloor;
}
