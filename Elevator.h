#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QObject>
#include <QTimer>
#include <QCheckBox>
#include "defs.h"
#include "DoorSensor.h"
#include "WeightSensor.h"
#include "AudioSystem.h"
#include "DisplayScreen.h"
#include "ControlPanel.h"
#include "FloorButtonPanel.h"

class Elevator : public QObject {
    Q_OBJECT
public:
    explicit Elevator(int elevatorID);
    ~Elevator() override;
    void passengerEnter();
    void passengerExit();
    bool addTargetFloor(int floor);
    ControlPanel* getControlPanel() const {return controlPanel;};
    bool isDoorOpen() const {return doorOpen;};
    bool isEmpty() const {return numPassengers == 0;};
    bool isIdle() const {return queuedTargetFloors.empty() && doorOpen == false && direction == IDLE;};
    std::queue<int>* getQueuedTargetFloors() {return &queuedTargetFloors;};

public slots:
    void move();
    void handleObstacle();
    void handleOverload();
    void handleFireAlarm();
    void handlePowerOut();
    void handleHelpSignal();
    void openDoor();
    void closeDoor();
    void keepDoorOpen();
    void closeDoorImmediately();

private slots:
    void processMove();
    void moveToSafeFloor();
    void ring();
    void halt();
    void switchToBattery();
    void displayPassengerCount() const;

private:
    int elevatorID;
    int currentFloor;
    int direction;
    int numPassengers;
    bool doorOpen;
    bool usingBattery;
    bool outOfService;
    QTimer moveTimer;
    QTimer doorTimer;
    std::queue<int> queuedTargetFloors;
    DoorSensor doorSensor;
    WeightSensor weightSensor;
    DisplayScreen display;
    AudioSystem audioSystem;
    ControlPanel* controlPanel;
};

#endif // ELEVATOR_H
