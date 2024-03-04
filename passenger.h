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
    void exitElevator();
    void pressFloorNumber(int floorNumber);
    int getRequiredFloorNumber() const {return requiredFloorNumber;}
    void setRequireFloorNumber(int number) {requiredFloorNumber = number;}

public slots:
    void updateCurrentFloor(Floor* newFloor);

signals:
    void messageReceived(const QString& message);
    void floorNumberPressed(int floorNumber, Elevator* elevator);


private:
    int passengerID;
    Floor* currentFloor;
    Elevator* currentElevator;
    bool beOutside;
    bool beInside;
    bool beWaitingForElevator;
    int requiredFloorNumber;
};

#endif // PASSENGER_H
