#include "Ecs.h"

void ECS::addPassenger(Passenger * p) {
    passengers.push_back(p);
}

void ECS::addElevator(Elevator * e) {
    elevators.push_back(e);
}

void ECS::addFloorRequest(const Floor* f, Direction d) {
    FloorRequest fr = FloorRequest{f, d};
    floorRequests.push_back(fr);
    if (d == Direction::UP)
        emit messageReceived("[ECS] Added floor request: Floor " + QString::number(f->getFloorNumber()) + " requests to go UP");
    else
        emit messageReceived("[ECS] Added floor request: Floor " + QString::number(f->getFloorNumber()) + " requests to go DOWN");
}

//void ECS::update() {
//    for (const FloorRequest& request : floorRequests) {
//            // Check if there's an idle elevator available
//            for (Elevator* elevator: elevators) {
//                if (elevator->getStatus() == Elevator::IDLE) {
//                    // Send the idle elevator to serve the request
//                    elevator->moveToFloor(request.floor);
//                    elevator->setStatus(Elevator::MOVING);
//                    removeFloorRequest(request);
//                    break; // Move to the next request
//                }
//            }
//        }
//}
