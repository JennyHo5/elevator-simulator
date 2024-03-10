#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QCoreApplication>
#include <QDebug>
#include "passenger.h"
#include "Ecs.h"
#include "Constants.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ECS;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow& getInstance(ECS* ecs);
    bool isSafetyServiceChecked() const {return safetyServiceChecked;}

private:
    explicit MainWindow(ECS* ecs, QWidget *parent = nullptr);
    ~MainWindow();
    // Declare the static instance variable
    static MainWindow* instance;

    // Private copy constructor and assignment operator to prevent cloning and assignment
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

    void connects(); //Connect singals and slots
    void illuminate(QPushButton*);
    void cancelIlluminate(QPushButton*);


private slots:
    void onOkButtonClicked();
    void onUpButtonClicked();
    void onDownButtonClicked();
    void onElevatorArrivedAtFloor(Elevator*, Floor*);

    void updateTextWidget(const QString& message);
    void update();

    void onFloor1ButtonClicked();
    void onFloor2ButtonClicked();
    void onFloor3ButtonClicked();
    void onFloor4ButtonClicked();
    void onFloor5ButtonClicked();
    void onFloor6ButtonClicked();
    void onFloor7ButtonClicked();

    void onOpenButtonClicked();
    void onCloseButtonClicked();
    void onHelpButtonClicked();
    void onDoorObstacleClicked();

    void onFireAlarmElevatorChecked();
    void onFireAlarmElevatorUnchecked();

    void onFireAlarmChecked();
    void onFireAlarmUnchecked();

    void onPoweroutChecked();
    void onPoweroutUnchecked();

    void onOverloadChecked();
    void onOverloadUnchecked();

    void displayTextOfDoorObstacle();


signals:
    void messageReceived(const QString& message);

private:
    Ui::MainWindow *ui;

    int selectedPassengerID;
    Passenger* selectedPassenger;
    Elevator* selectedElevator;
    ECS* ecs;
    QTimer* timer;


    bool safetyServiceChecked;
};
#endif // MAINWINDOW_H
