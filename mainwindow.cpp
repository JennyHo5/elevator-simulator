#include "mainwindow.h"
#include "ui_mainwindow.h"

// Initialize the static instance variable
MainWindow* MainWindow::instance = nullptr;

MainWindow& MainWindow::getInstance(ECS* ecs)
{
    // Create the instance if it doesn't exist
    if (!instance) {
        instance = new MainWindow(ecs);
    }
    return *instance;
}

MainWindow::MainWindow(ECS* ecs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ecs(ecs)
{
    selectedPassenger = nullptr;
    safetyServiceChecked = false;
    obstacleTimer = new QTimer(this);

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
    connect(ui->closeButton, SIGNAL(pressed()), this, SLOT(onCloseButtonClicked()));
    connect(ui->openButton, SIGNAL(pressed()), this, SLOT(onOpenButtonClicked()));
    connect(ui->helpButton, SIGNAL(pressed()), this, SLOT(onHelpButtonClicked()));
    connect(ui->passengerRespondCheckBox, &QCheckBox::stateChanged, this,
            [=](int state) {
              if (state == Qt::Checked) {
                  selectedElevator->setRespond(true);
              }
              else {
                  selectedElevator->setRespond(false);
              }
            });
    connect(ui->doorObstacleButton, SIGNAL(pressed()), this, SLOT(onDoorObstacleClicked()));
    connect(ui->elevatorFireAlarmButton, SIGNAL(pressed()), this, SLOT(onElevatorFireAlarmButtonClicked()));

    // Admin buttons
    connect(ui->safetyServiceCheckBox, &QCheckBox::stateChanged, this,
            [=](int state) {
              if (state == Qt::Checked) {
                safetyServiceChecked = true;
              }
              else {
                safetyServiceChecked = false;
              }
            });
    connect(ui->buildingFireButton, SIGNAL(pressed()), this, SLOT(onBuildingFireButtonClicked()));

    // Update console output text
    connect(this, &MainWindow::messageReceived, this, &MainWindow::updateTextWidget);
    connect(ecs, &ECS::messageReceived, this, &MainWindow::updateTextWidget);
    connect(ecs, &ECS::elevatorArrivedAtFloor, this, &MainWindow::onElevatorArrivedAtFloor);
    std::vector<Elevator*>* elevators = ecs->getElevators();
    for (auto it = elevators->begin(); it != elevators->end(); it++) {
        connect(*it, &Elevator::messageReceived, this, &MainWindow::updateTextWidget);
        connect(*it, &Elevator::obstacleWarned, this, &MainWindow::displayTextOfDoorObstacle);
        connect(*it, &Elevator::fireAlarmWarned, this, &MainWindow::displayTextOfFireAlarm);
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
        if (selectedPassenger->getCurrentElevator() != nullptr) // this will return segfault
        {
            selectedElevator = selectedPassenger->getCurrentElevator();
            ui->elevatorGroupBox->setEnabled(true);
            ui->floorGroupBox->setEnabled(false);
            ui->floorNumberLabel->setText("Current floor: " + QString::number(selectedPassenger->getCurrentElevator()->getCurrentFloor()->getFloorNumber()));
            if (selectedElevator->isDoorClosed())
                ui->doorObstacleButton->setEnabled(false);
            else
                ui->doorObstacleButton->setEnabled(true);
        }
        else
        {
            ui->floorGroupBox->setEnabled(true);
            ui->staticFloorNumberLabel->setText("Floor " + QString::number(selectedPassenger->getCurrentFloor()->getFloorNumber()));
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
    ui->upButton->setStyleSheet("background-color: green;");
    selectedPassenger->pressDirection(Direction::UP);
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::UP);
}

void MainWindow::onDownButtonClicked() {
    ui->downButton->setStyleSheet("background-color: green;");
    selectedPassenger->pressDirection(Direction::DOWN);
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::DOWN);
}

void MainWindow::onElevatorArrivedAtFloor(Elevator* e, Floor* f) {
    if (selectedPassenger->getCurrentFloor() != nullptr && f == selectedPassenger->getCurrentFloor())
    {
        ui->upButton->setStyleSheet("");
        ui->downButton->setStyleSheet("");
    }
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


void MainWindow::onOpenButtonClicked()
{
    if (selectedElevator && !selectedElevator->isDoorClosed()) {
        selectedElevator->openDoor();
    }
}


void MainWindow::onCloseButtonClicked()
{
    if (selectedElevator && !selectedElevator->isDoorClosed()) {
      selectedElevator->doorTimer->stop();
      selectedElevator->closeDoor(); // Call closeDoor immediately
    }
}


void MainWindow::onHelpButtonClicked()
{
    selectedPassenger->pressHelp();
}



void MainWindow::onDoorObstacleClicked()
{
    emit messageReceived("[Door Obstacle] Detected in Elevator " + QString::number(selectedElevator->getElevatorID()));
    if (!obstacleTimer->isActive()) // if the timer hasn't start or already ended, start a new one
    {
        obstacleTimer->start(5000);
        selectedElevator->openDoor();
    }
    else { //if the timer has start and hasn't stopped (within 5 seconds)
        obstacleTimer->stop();
        selectedElevator->warnObstacle();
    }
}

void MainWindow::displayTextOfDoorObstacle() {
    ui->warningLabel->setText("DOOR OBSTACLE");

    QTimer *tempTimer = new QTimer(this);

    connect(tempTimer, &QTimer::timeout, this, [=]() {
        ui->warningLabel->setText(""); // Reset the text after 3 seconds
        tempTimer->deleteLater();
    });

    tempTimer->start(3000);
}

void MainWindow::displayTextOfFireAlarm() {
    ui->warningLabel->setText("FIRE ALARM");

    QTimer *tempTimer = new QTimer(this);

    connect(tempTimer, &QTimer::timeout, this, [=]() {
        ui->warningLabel->setText(""); // Reset the text after 3 seconds
        tempTimer->deleteLater();
    });

    tempTimer->start(3000);
}





void MainWindow::onBuildingFireButtonClicked()
{
    ecs->recieveFireAlarmFromBuilding();
    displayTextOfFireAlarm();
}


void MainWindow::onElevatorFireAlarmButtonClicked()
{
    selectedElevator->warnFireAlarm(selectedElevator);
    displayTextOfFireAlarm();
}

