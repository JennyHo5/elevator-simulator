#include "Ecs.h"


void ECS::addPassenger(Passenger * p) {
    passengers.push_back(p);
}

void ECS::addElevator(Elevator * e) {
    elevators.push_back(e);
}

void ECS::addFloorRequest(Floor* f, Direction d) {
    FloorRequest fr = FloorRequest{f, d};
    floorRequests.push_back(fr);
    if (d == Direction::UP)
        emit messageReceived("[ECS] Added floor request: Floor " + QString::number(f->getFloorNumber()) + " requests to go UP");
    else
        emit messageReceived("[ECS] Added floor request: Floor " + QString::number(f->getFloorNumber()) + " requests to go DOWN");
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
        emit messageReceived("[ECS] Removed floor request: Floor " + QString::number(request->floor->getFloorNumber()) + " requests to go UP");
    else
        emit messageReceived("[ECS] Removed floor request: Floor " + QString::number(request->floor->getFloorNumber()) + " requests to go DOWN");
}

