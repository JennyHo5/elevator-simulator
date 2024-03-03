#ifndef PASSENGER_H
#define PASSENGER_H

#include "elevator.h"
#include <QTimer>

class Passenger: public QObject
{
    Q_OBJECT
public:
    Passenger(int id, Floor* currentFloor, QObject *parent = nullptr);
    ~Passenger();

    void pressDirection(Direction);
    bool isOutside() const {return beOutside;}
    bool isInside() const {return beInside;}
    bool isWaitingForElevator() const {return beWaitingForElevator;}

    Floor* getCurrentFloor() const {return currentFloor;}
    void enterElevator(Elevator*);

signals:
    void messageReceived(const QString& message);


private:
    int passengerID;
    Floor* currentFloor;
    Elevator* currentElevator;
    bool beOutside;
    bool beInside;
    bool beWaitingForElevator;
    Direction requireDirection;
};

#endif // PASSENGER_H
