#ifndef ECS_H
#define ECS_H

#include "passenger.h"
#include "FloorRequest.h"
#include "CarRequest.h"
#include "mainwindow.h"
#include <QTimer>
#include <vector>
#include <QDebug>

class MainWindow;

class ECS: public QObject
{
    Q_OBJECT

public:
    explicit ECS(QObject *parent = nullptr);
    ~ECS();

    void addPassenger(Passenger*);
    Passenger* getPassengerById(int id) {return passengers[id - 1];}
    void addElevator(Elevator*);
    void addFloor(Floor*);
    void addFloorRequest(Floor*, Direction);
    void removeFloorRequest(FloorRequest* request);
    void removeCarRequest(CarRequest* request);

    std::vector<Elevator*>* getElevators() {return &elevators;}
    std::vector<Passenger*>* getPassengers() {return &passengers;}
    std::vector<Floor*>* getFloors() {return &floors;}

    void moveElevatorToFloor(Elevator*, Floor*);
    void moveIdle();
    void movePassenger(); //check for passengers waiting on the same floor as an idle landing elevator and send them to the elevator
    void handleCarRequest();

    // Safety cases
    void callSafetyService(Elevator* e);
    void safetyServiceRespond();
    void call911();
    void recieveFireAlarmFromBuilding();
    void releaseFireAlarmFromBuilding();
    void recievePowerout();
    void releasePowerout();
    void recieveFireAlarmFromElevator(Elevator* e);

public slots:
    void update();
    void addCarRequest(int floorNumber, Elevator* e);


signals:
    void messageReceived(const QString& message);
    void elevatorArrivedAtFloor(Elevator* e, Floor* floor);

private:
    std::vector<Passenger*> passengers;
    std::vector<Elevator*> elevators;
    std::vector<Floor*> floors;
    std::vector<FloorRequest> floorRequests;
    std::vector<CarRequest> carRequests;
    QTimer *timer;
    QTimer* doorTimer;
    bool buildingFireAlarm;
    bool buildingPowerout;
};

#endif // ECS_H
