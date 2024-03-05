#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "floor.h"
#include <QDebug>
#include <QTimer>
#include <QEventLoop>

class Elevator: public QObject
{
    Q_OBJECT
public:
    enum {
        IDLE,
        MOVING,
        READY_TO_MOVE
    };

public:
    Elevator(int id, Floor* floor, QObject *parent = nullptr);
    int getStatus() const {return status;}
    void setStatus(int newStatus) {status = newStatus;}
    void setCurrentFloorNumber(int floorNumber);
    void setCurrentFloor(Floor* floor);
    void ringBell();
    void openDoor();
    void closeDoor();


    int getElevatorID() const {return elevatorID;}
    Floor* getCurrentFloor() const {return currentFloor;}

signals:
    void messageReceived(const QString& message);
    void currentFloorChanged(Floor* newFloor);

private:
    int elevatorID;
    Floor* currentFloor;
    int currentFloorNumber;
    int status;
};

#endif // ELEVATOR_H
