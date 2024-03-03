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


private slots:
    void onOkButtonClicked();
    void onUpButtonClicked();
    void onDownButtonClicked();
    void updateTextWidget(const QString& message);
    void update();

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
