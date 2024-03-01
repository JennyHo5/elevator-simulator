#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QCoreApplication>
#include <QDebug>
#include "passenger.h"
#include "Ecs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ECS* ecs, QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void onOkButtonClicked();
    void onUpButtonClicked();

private:
    Ui::MainWindow *ui;
    int selectedPassengerID;
    Passenger* selectedPassenger;
    ECS* ecs;
};
#endif // MAINWINDOW_H
