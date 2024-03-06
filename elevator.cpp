#include "Elevator.h"

Elevator::Elevator(int id, Floor* floor, QObject *parent): QObject(parent), elevatorID(id), currentFloor(floor), status(IDLE), doorClosed(true)
{

}


void Elevator::ringBell() {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Rings bell on Floor " + QString::number(currentFloor->getFloorNumber()));
}

void Elevator::openDoor() {
    doorClosed = false;
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Opens door on Floor " + QString::number(currentFloor->getFloorNumber()));
}

void Elevator::closeDoor() {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Closes door on Floor " + QString::number(currentFloor->getFloorNumber()));
    doorClosed = true;
}

void Elevator::setCurrentFloor(Floor* floor) {
    // Create a QEventLoop object
    QEventLoop loop;

    // Connect the timer's single-shot timeout signal to the loop's quit() slot
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    // Set the timer interval to 3 seconds
    timer->start(1000); // 3000 milliseconds = 3 seconds

    // Set the current floor and enter the event loop
    currentFloor = floor;
    loop.exec(); // This line blocks until the timer times out or other events occur
}

