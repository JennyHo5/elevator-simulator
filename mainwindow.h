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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ECS* ecs, QWidget *parent = nullptr);
    ~MainWindow();

    void connects(); //Connect singals and slots


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

signals:
    void messageReceived(const QString& message);

private:
    Ui::MainWindow *ui;

    int selectedPassengerID;
    Passenger* selectedPassenger;
    ECS* ecs;
    QTimer* timer;
};
#endif // MAINWINDOW_H
