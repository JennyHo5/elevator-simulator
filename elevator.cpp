#include "Elevator.h"

Elevator::Elevator(int id, Floor* floor, QObject *parent): QObject(parent), elevatorID(id), currentFloor(floor), status(IDLE), doorClosed(true)
{
    doorTimer = new QTimer(this);
    hasRespond = false;
    // Connect the timeout signal of the timer to a slot that will close the door and set the elevator status
    connect(doorTimer, &QTimer::timeout, this, [=]() {
        closeDoor();
        setStatus(Elevator::IDLE);
    });
}


void Elevator::ringBell() {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Rings bell on Floor " + QString::number(currentFloor->getFloorNumber()));
}

void Elevator::openDoor() {
    ringBell();
    doorClosed = false;
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Opens door on Floor " + QString::number(currentFloor->getFloorNumber()));

    if (doorTimer) {
      doorTimer->stop(); // Stop if already running
      doorTimer->start(10000); // Restart the timer
    }
}

void Elevator::closeDoor() {
    ringBell();
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Closes door on Floor " + QString::number(currentFloor->getFloorNumber()));
    doorClosed = true;
    doorTimer->stop();
}

void Elevator::setCurrentFloor(Floor* floor) {
    // Create a QEventLoop object
    QEventLoop loop;

    // Connect the timer's single-shot timeout signal to the loop's quit() slot
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    // Set the timer interval to 2 seconds
    timer->start(2000);

    // Set the current floor and enter the event loop
    currentFloor = floor;
    loop.exec(); // This line blocks until the timer times out or other events occur
}

void Elevator::warnObstacle() {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Warns door obstacle on audio");
    emit obstacleWarned();
}

void Elevator::warnFireAlarm(Elevator* e) {
    emit messageReceived("[Elevator " + QString::number(elevatorID) + "] Warns fire alarm on audio");
    emit fireAlarmWarned(e);
}
