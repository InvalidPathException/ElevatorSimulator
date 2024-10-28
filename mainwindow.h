#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QLCDNumber>
#include "Coordinator.h"

class Elevator;
class FloorButtonPanel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    static MainWindow* getMainWindow() { return singleton; }
    void addEventLog(const std::string& event);
    void clearLayout(QLayout* layout);
    void updateDisplay(int elevatorID, int floor, int direction);
    int getNumFloors() const { return numFloors; }
    int getNumElevators() const { return numElevators; }
    int getSafeFloor() const { return safeFloor; }

public slots:

private:
    static MainWindow* singleton;
    Ui::MainWindow *ui;
    Coordinator* coordinator;
    // Another copy of the numbers needed to make Coordinator initialization easier
    int numFloors;
    int numElevators;
    int safeFloor;

private slots:
    void startSimulationHandler();
    void numFloorsSpinHandler();
    void outageButtonHandler();
    void fireAlarmButtonHandler();
    void fromFloorSpinHandler();
    void elevatorSelectionSpinHandler();
    void upButtonHandler();
    void downButtonHandler();
    void passengerEnterHandler();
    void passengerExitHandler();
    void targetFloorButtonHandler();
    void helpButtonHandler();
    void doorCloseHandler();
    void doorOpenPressHandler();
    void doorOpenReleaseHandler();
    void localFireAlarmHandler();
};
#endif // MAINWINDOW_H
