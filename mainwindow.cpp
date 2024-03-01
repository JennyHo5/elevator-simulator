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
    emit messageReceived("[Passenger " + QString::number(selectedPassengerID) + "] Pressed UP button on floor " + QString::number(selectedPassenger->getCurrentFloor()->getFloorNumber()));
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::UP);
    emit messageReceived("[ECS] Added floor request: Floor " + QString::number(selectedPassenger->getCurrentFloor()->getFloorNumber()) + " requests to go UP");
}

void MainWindow::onDownButtonClicked() {
    emit messageReceived("[Passenger " + QString::number(selectedPassengerID) + "] Pressed DOWN button on floor " + QString::number(selectedPassenger->getCurrentFloor()->getFloorNumber()));
    ecs->addFloorRequest(selectedPassenger->getCurrentFloor(), Direction::DOWN);
    emit messageReceived("[ECS] Added floor request: Floor " + QString::number(selectedPassenger->getCurrentFloor()->getFloorNumber()) + " requests to go DOWN");
}

void MainWindow::updateTextWidget(const QString& message) {
    ui->outputText->append(message);
}
