#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "floor.h"
#include <QDebug>
#include <QTimer>

class Elevator: public QObject
{
    Q_OBJECT
public:
    enum {
        IDLE,
        MOVING
    };

public:
    Elevator(int id, Floor* floor, QObject *parent = nullptr);
    int getStatus() const {return status;}
    void setStatus(int newStatus) {status = newStatus;}
    void setCurrentFloorNumber(int floorNumber) {currentFloorNumber = floorNumber;}
    void setCurrentFloor(Floor* floor) {currentFloor = floor;}
    void ringBell();
    void openDoor();


    int getElevatorID() const {return elevatorID;}
    Floor* getCurrentFloor() const {return currentFloor;}

signals:
    void messageReceived(const QString& message);

private:
    int elevatorID;
    Floor* currentFloor;
    int currentFloorNumber;
    int status;
};

#endif // ELEVATOR_H
