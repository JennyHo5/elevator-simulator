#include "Elevator.h"

Elevator::Elevator(int id, Floor* floor, QObject *parent): QObject(parent), elevatorID(id), currentFloor(floor), status(IDLE)
{

}

void Elevator::moveToFloor(Floor* f) {
    // if the elevator is already on floor f
    if (f == currentFloor) {
        emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Is already on Floor " + QString::number(f->getFloorNumber()));
    }
    // if the elevator need to move
    else {
        emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Moving to Floor " + QString::number(f->getFloorNumber()));
        status = MOVING;
        // Create a QTimer object
        QTimer* timer = new QTimer(this);
        // Set a single-shot timer with a 3-second interval
        timer->setSingleShot(true);
        timer->start(3000); // 3000 milliseconds = 3 seconds

        // Connect the timeout signal of the timer to a lambda function
        connect(timer, &QTimer::timeout, [this, f, timer]() {
            // This code block will be executed after 3 seconds
            emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Arrived at Floor " + QString::number(f->getFloorNumber()));
            // Cleanup: delete the timer
            ringBell();
            openDoor();
            timer->deleteLater();
        });

        status = IDLE;
        currentFloor = f;
    }
}

void Elevator::ringBell() {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Rings bell");
}

void Elevator::openDoor() {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Opens door");
}
