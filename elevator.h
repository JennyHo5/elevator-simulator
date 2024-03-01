#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "floor.h"

class Elevator
{

public:
    enum {
        IDLE,
        MOVING
    };

public:
    Elevator(int id, Floor* floor);
    int getStatus() const {return status;}
    void setStatus(int newStatus) {status = newStatus;}
    void moveToFloor(const Floor*);

private:
    int elevatorID;
    Floor* currentFloor;
    int status;
};

#endif // ELEVATOR_H
