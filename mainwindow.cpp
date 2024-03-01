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
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(onDownButtonClicked()));
    connect(this, &MainWindow::messageReceived, this, &MainWindow::updateTextWidget);
    connect(ecs, &ECS::messageReceived, this, &MainWindow::updateTextWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onOkButtonClicked()
{
    selectedPassengerID = ui->passengerSpinBox->value();
    selectedPassenger = ecs->getPassengerById(selectedPassengerID);
    emit messageReceived("[Admin] Selected passenger " + QString::number(selectedPassengerID));

    // Enable the group on GUI based on where passenger is
    if (selectedPassenger->isInside())
    {
        ui->elevatorGroupBox->setEnabled(true);
        ui->floorGroupBox->setEnabled(false);
    }
    else if (selectedPassenger->isOutside())
    {
        ui->floorGroupBox->setEnabled(true);
        ui->elevatorGroupBox->setEnabled(false);
        // Disable button UP or DOWN based on floor
        if (selectedPassenger->getCurrentFloor()->getFloorNumber() == 1)
            ui->downButton->setEnabled(false);
        else if (selectedPassenger->getCurrentFloor()->getFloorNumber() == NUM_OF_ELEVATORS)
            ui->upButton->setEnabled(false);
    }
}

void MainWindow::onUpButtonClicked()
{
    emit messageReceived("[Passenger " + QString::number(selectedPassengerID) + "] Pressed UP button on floor " + QString::number(selectedPassenger->getCurrentFloor()->getFloorNumber()));
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::UP);
}

void MainWindow::onDownButtonClicked() {
    emit messageReceived("[Passenger " + QString::number(selectedPassengerID) + "] Pressed DOWN button on floor " + QString::number(selectedPassenger->getCurrentFloor()->getFloorNumber()));
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::DOWN);
}

void MainWindow::updateTextWidget(const QString& message) {
    ui->outputText->append(message);
}
