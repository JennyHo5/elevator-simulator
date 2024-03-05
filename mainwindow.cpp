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
    connect(ui->floor2Button, SIGNAL(pressed()), this, SLOT(onFloor2ButtonClicked()));
    connect(ui->floor3Button, SIGNAL(pressed()), this, SLOT(onFloor3ButtonClicked()));
    connect(ui->floor4Button, SIGNAL(pressed()), this, SLOT(onFloor4ButtonClicked()));
    connect(ui->floor5Button, SIGNAL(pressed()), this, SLOT(onFloor5ButtonClicked()));
    connect(ui->floor6Button, SIGNAL(pressed()), this, SLOT(onFloor6ButtonClicked()));
    connect(ui->floor7Button, SIGNAL(pressed()), this, SLOT(onFloor7ButtonClicked()));


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
            if (selectedPassenger->getCurrentFloor()->getFloorNumber() == 1) {
                ui->downButton->setEnabled(false);
                ui->upButton->setEnabled(true);
            }
            else if (selectedPassenger->getCurrentFloor()->getFloorNumber() == NUM_OF_ELEVATORS) {
                ui->downButton->setEnabled(true);
                ui->upButton->setEnabled(false);
            }
            else {
                ui->downButton->setEnabled(true);
                ui->upButton->setEnabled(true);
            }
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

void MainWindow::onFloor2ButtonClicked()
{
    selectedPassenger->pressFloorNumber(2);
}


void MainWindow::onFloor3ButtonClicked()
{
    selectedPassenger->pressFloorNumber(3);
}


void MainWindow::onFloor4ButtonClicked()
{
    selectedPassenger->pressFloorNumber(4);
}


void MainWindow::onFloor5ButtonClicked()
{
    selectedPassenger->pressFloorNumber(5);
}


void MainWindow::onFloor6ButtonClicked()
{
    selectedPassenger->pressFloorNumber(6);
}


void MainWindow::onFloor7ButtonClicked()
{
    selectedPassenger->pressFloorNumber(7);
}

