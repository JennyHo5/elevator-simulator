#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ECS* ecs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ecs(ecs)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(onOkButtonClicked()));
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(onUpButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onOkButtonClicked()
{
    selectedPassengerID = ui->passengerSpinBox->value();
    selectedPassenger = ecs->getPassengerById(selectedPassengerID);
    qInfo("Selected passenger:");
    selectedPassenger->print();
    if (selectedPassenger->isInside())
    {
        ui->elevatorGroupBox->setEnabled(true);
        ui->floorGroupBox->setEnabled(false);
    }
    else if (selectedPassenger->isOutside())
    {
        ui->floorGroupBox->setEnabled(true);
        ui->elevatorGroupBox->setEnabled(false);
    }
}

void MainWindow::onUpButtonClicked()
{
    qInfo("Passenger %d pressed UP button on floor %d", selectedPassengerID, selectedPassenger->getCurrentFloor()->getFloorNumber());
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::UP);
}

