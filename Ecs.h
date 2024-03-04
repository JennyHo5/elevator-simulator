#ifndef ECS_H
#define ECS_H

#include "passenger.h"
#include "FloorRequest.h"
#include "CarRequest.h"
#include <QTimer>
#include <vector>
#include <QDebug>

class ECS: public QObject
{
    Q_OBJECT

public:
    explicit ECS(QObject *parent = nullptr);

    ~ECS() {
        delete timer;
    }
    void addPassenger(Passenger*);
    Passenger* getPassengerById(int id) {return passengers[id - 1];}
    void addElevator(Elevator*);
    void addFloor(Floor*);
    void addFloorRequest(Floor*, Direction);
    void removeFloorRequest(FloorRequest* request);

    std::vector<Elevator*>* getElevators() {return &elevators;}
    std::vector<Passenger*>* getPassengers() {return &passengers;}
    void moveIdle();
    void movePassenger(); //check for passengers waiting on the same floor as an idle landing elevator and send them to the elevator

public slots:
    void update();
    void addCarRequest(int floorNumber, Elevator* e);


signals:
    void messageReceived(const QString& message);

private:
    std::vector<Passenger*> passengers;
    std::vector<Elevator*> elevators;
    std::vector<Floor*> floors;
    std::vector<FloorRequest> floorRequests;
    std::vector<CarRequest> carRequests;
    QTimer *timer;
};

#endif // ECS_H
