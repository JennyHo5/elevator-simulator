#include "Ecs.h"

ECS::ECS(QObject *parent): QObject(parent) {
    // a QTimer that triggers the update() method every second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ECS::update);
    timer->start(1000); // Adjust the interval as needed (in milliseconds)
    buildingFireAlarm = false;
    buildingPowerout = false;
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
    connect(e, &Elevator::overloadReleased, this, &ECS::handleCarRequest);
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
    movePassenger();
    moveIdle(); //handle floor request
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
            // if elevator has received a floor alarm, stop immidiantly
            if (e->getFireAlarm()) {
                emit messageReceived("[ECS] Stops Elevator " + QString::number(e->getElevatorID()) + " on Floor " + QString::number(e->getCurrentFloor()->getFloorNumber()) + " due to fire alarm");
                break;
            }

            // if elevator has received a powerout, stop immidiantly
            if (e->getPowerout()) {
                emit messageReceived("[ECS] Stops Elevator " + QString::number(e->getElevatorID()) + " on Floor " + QString::number(e->getCurrentFloor()->getFloorNumber()) + " due to powerout");
                break;
            }

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
            if (passenger->getCurrentElevator() != nullptr)
            {
                if (passenger->getCurrentFloor()->getFloorNumber() == passenger->getRequiredFloorNumber() && passenger->isInside()) {
                    passenger->exitElevator();
                } else if ((buildingFireAlarm || buildingPowerout || passenger->getCurrentElevator()->getFireAlarm()) && !passenger->getCurrentElevator()->isDoorClosed()) {// else if powerout and fire, and the door is open, then exit
                    passenger->exitElevator();
                }
            }
        }
    }
}

void ECS::handleCarRequest() {
    for (size_t i = 0; i < carRequests.size(); i++) {
        if (carRequests[i].elevator->getOverload()) { // if the elevator is overload, don't handle the car request
            break;
        }
        if (carRequests[i].elevator->isDoorClosed()) {
            moveElevatorToFloor(carRequests[i].elevator, carRequests[i].floor);
            removeCarRequest(&carRequests[i]);
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
    emit messageReceived("[ECS] Removes car request: Elevator " + QString::number(request->elevator->getElevatorID()) + " requests to go to Floor " + QString::number(request->floor->getFloorNumber()));

    size_t i = 0;
    for (; i < carRequests.size(); i++) {
        if(&carRequests[i] == request) {
            break;
        }
    }
    if (i < carRequests.size()) {
        carRequests.erase(carRequests.begin() + i);
    }
}

void ECS::callSafetyService(Elevator *e) {
    emit messageReceived("[ECS] Calling safety service...");
    QTimer* safetyServiceTimer = new QTimer(this);

    connect(safetyServiceTimer, &QTimer::timeout, this, [=]() {
        safetyServiceTimer->deleteLater();

        // Check if user checked the "safety service" checkbox within 5 seconds (replace with actual user interaction logic)
        if (MainWindow::getInstance(this).isSafetyServiceChecked()) {
            emit messageReceived("[ECS] Connects voice of the current passenger to safety service");
            // if no response from the passenger, call 911
            if (!e->getRespond()) {
                emit messageReceived("[ECS] No response from Passenger. Calling 911..."); // Placeholder for alternate action
            }
            else {
                emit messageReceived("[Elevator " + QString::number(e->getElevatorID()) + "] Responds to Safety Service");
            }
        } else {
            emit messageReceived("[ECS] Safety service unavailable. Calling 911..."); // Placeholder for alternate action
        }
    });

    safetyServiceTimer->start(5000);
}

void ECS::recieveFireAlarmFromBuilding() {
    buildingFireAlarm = true;
    for (Elevator* e: elevators) {
        e->setFireAlarm(true);
    }
    emit messageReceived("[ECS] Warns fire alarm from the building on audio, moving every elevator to a safe floor");
}

void ECS::releaseFireAlarmFromBuilding() {
    buildingFireAlarm = false;
    for (Elevator* e: elevators) {
        e->setFireAlarm(false);
    }
}

void ECS::recievePowerout() {
    buildingPowerout = true;
    for (Elevator *e: elevators) {
        e->setPowerout(true);
    }
    emit messageReceived("[ECS] Warns powerout on audio, moving every elevator to a safe floor");
}

void ECS::releasePowerout() {
    buildingPowerout = false;
    for (Elevator* e: elevators) {
        e->setPowerout(false);
    }
}

void ECS::recieveFireAlarmFromElevator(Elevator *e) {
    emit messageReceived("[ECS] Moving elevator " + QString::number(e->getElevatorID()) + " to Floor 1 (safe floor)");
    e->setFireAlarm(true);
}

