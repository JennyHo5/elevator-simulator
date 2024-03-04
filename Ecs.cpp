#include "Ecs.h"

ECS::ECS(QObject *parent): QObject(parent) {
    // a QTimer that triggers the update() method every second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ECS::update);
    timer->start(1000); // Adjust the interval as needed (in milliseconds)
}

void ECS::addPassenger(Passenger * p) {
    passengers.push_back(p);
    // Connect ECS with Passenger
    connect(p, &Passenger::floorNumberPressed, this, &ECS::addCarRequest);
}

void ECS::addElevator(Elevator * e) {
    elevators.push_back(e);
}

void ECS::addFloor(Floor * f) {
    floors.push_back(f);
}

void ECS::addFloorRequest(Floor* f, Direction d) {
    FloorRequest fr = FloorRequest{f, d};
    floorRequests.push_back(fr);
    if (d == Direction::UP)
        emit messageReceived("[ECS] Adds floor request: Floor " + QString::number(f->getFloorNumber()) + " requests to go UP");
    else
        emit messageReceived("[ECS] Adds floor request: Floor " + QString::number(f->getFloorNumber()) + " requests to go DOWN");
}

void ECS::update() {
    moveIdle();
    movePassenger();
}

void ECS::moveIdle() {
    for (FloorRequest& request: floorRequests) {
            // Check if there's an idle elevator available
            for (Elevator* elevator: elevators) {
                if (elevator->getStatus() == Elevator::IDLE) {
                    // Send the idle elevator to serve the request
                    elevator->moveToFloor(request.floor);
                    removeFloorRequest(&request);
                    break; // Move to the next request
                }
            }
        }
}

void ECS::movePassenger() {
    for (Passenger* passenger: passengers) {
        if (passenger->isWaitingForElevator()) {
            for (Elevator* elevator: elevators) {
                // If there is an idle elevator that is on the same floor of the passenger, serve the passenger with the elevator
                if (elevator->getStatus() == Elevator::IDLE && elevator->getCurrentFloor() == passenger->getCurrentFloor()) {
                    passenger->enterElevator(elevator);
                    break;
                }
            }
        }
    }
}

void ECS::removeFloorRequest(FloorRequest* request) {
    size_t i = 0;
    for (; i < floorRequests.size(); i++) {
        if(&floorRequests[i] == request) {
            break;
        }
    }
    if (i < floorRequests.size()) {
        floorRequests.erase(floorRequests.begin() + i);
    }
    if (request->direction == Direction::UP)
        emit messageReceived("[ECS] Removes floor request: Floor " + QString::number(request->floor->getFloorNumber()) + " requests to go UP");
    else
        emit messageReceived("[ECS] Removes floor request: Floor " + QString::number(request->floor->getFloorNumber()) + " requests to go DOWN");
}

void ECS::addCarRequest(int floorNumber, Elevator* e) {
    CarRequest cr = CarRequest{floors[floorNumber], e};
    carRequests.push_back(cr);
    emit messageReceived("[ECS] Adds car request: Elevator " + QString::number(e->getElevatorID()) + " requests to go to Floor " + QString::number(floorNumber));
}

