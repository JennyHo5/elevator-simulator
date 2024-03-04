#include "Elevator.h"

Elevator::Elevator(int id, Floor* floor, QObject *parent): QObject(parent), elevatorID(id), currentFloor(floor), status(IDLE)
{

}


void Elevator::ringBell() {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Rings bell on Floor " + QString::number(currentFloor->getFloorNumber()));
}

void Elevator::openDoor() {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Opens door on Floor " + QString::number(currentFloor->getFloorNumber()));
}
