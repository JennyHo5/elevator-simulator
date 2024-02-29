#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ECS* ecs, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ecs(ecs)
{
    ui->setupUi(this);
    connect(ui->okButton, SIGNAL(pressed()), this, SLOT (onOkButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onOkButtonClicked()
{
    int selectedPassengerID = ui->passengerSpinBox->value();
    qDebug() << "Selected passenger" << selectedPassengerID;
    selectedPassenger = ecs->getPassengerById(selectedPassengerID);
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

