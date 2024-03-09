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
    };

public:
    Elevator(int id, Floor* floor, QObject *parent = nullptr);


    bool isDoorClosed() const {return doorClosed;}
    void ringBell();
    void openDoor();
    void closeDoor();
    void stop() {status = IDLE;}

    void warnObstacle();
    void receiveFireAlarm();
    void releaseFireAlarm();

    void receiveOverload();
    void releaseOverload();

    // getters and setters
    int getStatus() const {return status;}
    void setStatus(int newStatus) {status = newStatus;}
    void setCurrentFloorNumber(int floorNumber);
    void setCurrentFloor(Floor* floor);
    int getElevatorID() const {return elevatorID;}
    Floor* getCurrentFloor() const {return currentFloor;}
    void setRespond(bool r) {hasRespond = r;}
    bool getRespond() const {return hasRespond;}
    void setFireAlarm(bool f) {hasFireAlarm = f;}
    bool getFireAlarm() const {return hasFireAlarm;}
    void setPowerout(bool p) {isPowerout = p;}
    bool getPowerout() const {return isPowerout;}
    bool getOverload() const {return isOverload;}


signals:
    void messageReceived(const QString& message);
    void currentFloorChanged(Floor* newFloor);
    void obstacleWarned();
    void overloadReleased();

private:
    int elevatorID;
    Floor* currentFloor;
    int currentFloorNumber;
    int status;
    bool doorClosed;
    bool hasRespond;
    bool hasFireAlarm;
    bool isOverload;
    bool isPowerout;

public:
    QTimer* doorTimer;
};

#endif // ELEVATOR_H
