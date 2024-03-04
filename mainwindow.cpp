#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ECS* ecs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ecs(ecs)
{
    // Timer for update
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update); // a QTimer that triggers the update() method every second
    timer->start(1000); // Adjust the interval as needed (in milliseconds)

    ui->setupUi(this);
    connects();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connects() {
    // Floor buttons
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT(onOkButtonClicked()));
    connect(ui->upButton, SIGNAL(pressed()), this, SLOT(onUpButtonClicked()));
    connect(ui->downButton, SIGNAL(pressed()), this, SLOT(onDownButtonClicked()));

    // Elevator buttons
    connect(ui->floor1Button, SIGNAL(pressed()), this, SLOT(onFloor1ButtonClicked()));

    // Update console output text
    connect(this, &MainWindow::messageReceived, this, &MainWindow::updateTextWidget);
    connect(ecs, &ECS::messageReceived, this, &MainWindow::updateTextWidget);
    std::vector<Elevator*>* elevators = ecs->getElevators();
    for (auto it = elevators->begin(); it != elevators->end(); it++) {
        connect(*it, &Elevator::messageReceived, this, &MainWindow::updateTextWidget);
    }
    std::vector<Passenger*>* passengers = ecs->getPassengers();
    for (auto it = passengers->begin(); it != passengers->end(); it++) {
        connect(*it, &Passenger::messageReceived, this, &MainWindow::updateTextWidget);
    }

}


void MainWindow::onOkButtonClicked()
{
    selectedPassengerID = ui->passengerSpinBox->value();
    selectedPassenger = ecs->getPassengerById(selectedPassengerID);
    emit messageReceived("[Admin] Selected passenger " + QString::number(selectedPassengerID));
}

void MainWindow::update() {
    if (selectedPassenger != nullptr) {
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
}

void MainWindow::onUpButtonClicked()
{
    selectedPassenger->pressDirection(Direction::UP);
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::UP);
}

void MainWindow::onDownButtonClicked() {
    selectedPassenger->pressDirection(Direction::DOWN);
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::DOWN);
}

void MainWindow::updateTextWidget(const QString& message) {
    ui->outputText->append(message);
}


void MainWindow::onFloor1ButtonClicked()
{
    selectedPassenger->pressFloorNumber(1);
}


