#include "mainwindow.h"
#include "Passenger.h"
#include "Elevator.h"
#include "Floor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initalize 3 Passengers, 3 Elevators and 7 Floors
    Passenger passenger1(1);
    Passenger passenger2(2);
    Passenger passenger3(3);
    Elevator elevator1(1);
    Elevator elevator2(2);
    Elevator elevator3(3);
    Elevator elevator4(4);
    Elevator elevator5(5);
    Elevator elevator6(6);
    Elevator elevator7(7);
    Floor floor1(1);
    Floor floor2(2);
    Floor floor3(3);

    MainWindow w;
    w.show();


    return a.exec();
}
