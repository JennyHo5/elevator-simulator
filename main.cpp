#include "mainwindow.h"
#include "Passenger.h"
#include "Elevator.h"
#include "Floor.h"
#include "Ecs.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initalize 7 floors
    Floor floor1(1);
    Floor floor2(2);
    Floor floor3(3);
    Floor floor4(4);
    Floor floor5(5);
    Floor floor6(6);
    Floor floor7(7);

    // 3 Elevators are initaly at Floor 1
    Elevator elevator1(1, &floor1);
    Elevator elevator2(2, &floor1);
    Elevator elevator3(3, &floor1);

    // 3 Passengers are initaly at Floor 1
    Passenger passenger1(1, &floor1);
    Passenger passenger2(2, &floor1);
    Passenger passenger3(3, &floor1);

    ECS ecs;
    ecs.addElevator(&elevator1);
    ecs.addElevator(&elevator2);
    ecs.addElevator(&elevator3);
    ecs.addPassenger(&passenger1);
    ecs.addPassenger(&passenger2);
    ecs.addPassenger(&passenger3);
    ecs.addFloor(&floor1);
    ecs.addFloor(&floor2);
    ecs.addFloor(&floor3);
    ecs.addFloor(&floor4);
    ecs.addFloor(&floor5);
    ecs.addFloor(&floor6);
    ecs.addFloor(&floor7);

    MainWindow w(&ecs);
    w.show();


    return a.exec();
}
