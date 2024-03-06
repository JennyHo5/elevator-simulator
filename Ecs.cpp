#include "Ecs.h"

ECS::ECS(QObject *parent): QObject(parent) {
    // a QTimer that triggers the update() method every second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ECS::update);
    timer->start(1000); // Adjust the interval as needed (in milliseconds)
}

ECS::~ECS() {
    delete timer;
}

void ECS::addPassenger(Passenger * p) {
    passengers.push_back(p);
    // Connect ECS with Passenger
    connect(p, &Passenger::floorNumberPressed, this, &ECS::addCarRequest);
    connect(p, &Passenger::helpButtonPressed, this, &ECS::callSafetyService);
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
    handleCarRequest();
}

void ECS::moveIdle() {
    for (FloorRequest& request: floorRequests) {
            Elevator* closestElevator = nullptr;
            int closestElevatorGap = 8;
            // Check if there's an idle elevator available
            for (Elevator* elevator: elevators) {
                if (elevator->getStatus() == Elevator::IDLE) {
                    // Send the idle elevator that is the closest to the requested floor
                    if (abs(elevator->getCurrentFloor()->getFloorNumber() - request.floor->getFloorNumber()) < closestElevatorGap) {
                        closestElevator = elevator;
                        closestElevatorGap = abs(elevator->getCurrentFloor()->getFloorNumber() - request.floor->getFloorNumber());
                    }
                }
            }
            if (closestElevator != nullptr) {
                emit messageReceived("[ECS] Chooses Elevator " + QString::number(closestElevator->getElevatorID()) + " to serve the request");
                moveElevatorToFloor(closestElevator, request.floor);
                removeFloorRequest(&request);
            }
        }
}

void ECS::moveElevatorToFloor(Elevator *e, Floor *f) {
    if (e->getCurrentFloor() == f) {
        emit messageReceived("[Elevator " + QString::number(e->getElevatorID()) + "] Is already on Floor " + QString::number(f->getFloorNumber()));
        e->openDoor();
    } else {
        int elevatorFloorNumber = e->getCurrentFloor()->getFloorNumber();
        int endFloorNumber = f->getFloorNumber();
        int floorsToMove = std::abs(elevatorFloorNumber - endFloorNumber);
        // Set status to MOVING
        e->setStatus(Elevator::MOVING);

        // Move floor by floor
        for (int i = 0; i < floorsToMove; i++) {
            if (elevatorFloorNumber > endFloorNumber) {
                elevatorFloorNumber--;
                e->setCurrentFloor(floors[elevatorFloorNumber - 1]);
            } else {
                elevatorFloorNumber++;
                e->setCurrentFloor(floors[elevatorFloorNumber - 1]);
            }
            emit messageReceived("[Elevator " + QString::number(e->getElevatorID()) + "] Arrives on Floor " + QString::number(elevatorFloorNumber));
            emit e->currentFloorChanged(e->getCurrentFloor()); // Emit the signal when the current floor changes
        }
        e->openDoor();        
    }

    // Emit the signal indicating elevator has arrived at the destination floor
    emit elevatorArrivedAtFloor(e, f);
}


void ECS::movePassenger() {
    for (Passenger* passenger: passengers) {
        // Move passenger that is waiting for elevator into idle elevator
        if (passenger->isWaitingForElevator() && passenger->isOutside()) {
            for (Elevator* elevator: elevators) {
                // If there is an idle elevator that is on the same floor of the passenger, serve the passenger with the elevator
                if (!elevator->isDoorClosed() && elevator->getCurrentFloor() == passenger->getCurrentFloor()) {
                    passenger->enterElevator(elevator);
                    break;
                }
            }
        }
        else {
            // Move passenger that arrives on the destinated floor out
            if (passenger->getCurrentFloor()->getFloorNumber() == passenger->getRequiredFloorNumber() && passenger->isInside()) {
                passenger->exitElevator();
            }
        }
    }
}

void ECS::handleCarRequest() {
    for (CarRequest& cr: carRequests) {
        if (cr.elevator->isDoorClosed()) {
            moveElevatorToFloor(cr.elevator, cr.floor);
            removeCarRequest(&cr);
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
    CarRequest cr = CarRequest{floors[floorNumber - 1], e};
    carRequests.push_back(cr);
    emit messageReceived("[ECS] Adds car request: Elevator " + QString::number(e->getElevatorID()) + " requests to go to Floor " + QString::number(floorNumber));
}

void ECS::removeCarRequest(CarRequest *request) {
    size_t i = 0;
    for (; i < carRequests.size(); i++) {
        if(&carRequests[i] == request) {
            break;
        }
    }
    if (i < carRequests.size()) {
        carRequests.erase(carRequests.begin() + i);
    }

    emit messageReceived("[ECS] Removes car request: Elevator " + QString::number(request->elevator->getElevatorID()) + " requests to go to Floor " + QString::number(request->floor->getFloorNumber()));
}

void ECS::callSafetyService(Elevator *e) {
    emit messageReceived("[ECS] Calling safety service...");
    QTimer* safetyServiceTimer = new QTimer(this);

    connect(safetyServiceTimer, &QTimer::timeout, this, [=]() {
        safetyServiceTimer->deleteLater();

        // Check if user checked the "safety service" checkbox within 5 seconds (replace with actual user interaction logic)
        if (MainWindow::getInstance(this).isSafetyServiceChecked()) {
            emit messageReceived("[ECS] Connects voice of the current passenger to safety service");
            // wait for 5 seconds, if no response from the passenger, call 911
            QTimer* tempTimer = new QTimer(this);
            connect(tempTimer, &QTimer::timeout, this, [=]() {
                tempTimer->deleteLater();
                if (!e->getRespond()) {
                    emit messageReceived("[ECS] No response from Passenger. Calling 911..."); // Placeholder for alternate action
                }
                else {
                    emit messageReceived("[Elevator " + QString::number(e->getElevatorID()) + "] Responds to Safety Service");
                }
            });
            tempTimer -> start(5000);
        } else {
            emit messageReceived("[ECS] Safety service unavailable. Calling 911..."); // Placeholder for alternate action
        }
    });

    safetyServiceTimer->start(5000);
}

