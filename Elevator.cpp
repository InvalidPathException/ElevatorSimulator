#include "Elevator.h"

Elevator::Elevator(int elevatorID) : QObject(), elevatorID(elevatorID), currentFloor(0), direction(0), numPassengers(0), doorOpen(false), usingBattery(false), outOfService(false), display(elevatorID), audioSystem(elevatorID), controlPanel(new ControlPanel(this)){
    // Connect the timers to their respective slots
    connect(&moveTimer, &QTimer::timeout, this, &Elevator::processMove);
    connect(&doorTimer, &QTimer::timeout, this, &Elevator::closeDoor);
    moveTimer.setSingleShot(true);
    doorTimer.setSingleShot(true);
}

Elevator::~Elevator() {
    delete controlPanel;
}

bool Elevator::addTargetFloor(int floor) {
    if (outOfService) return false;
    queuedTargetFloors.push(floor);
    return true;
}

void Elevator::move() {
    if (queuedTargetFloors.empty()) return;
    int targetFloor = queuedTargetFloors.front();
    // If the elevator is already at the target floor, open the door
    if (targetFloor == currentFloor) {
        queuedTargetFloors.pop();
        halt();
        return;
    }
    direction = (targetFloor > currentFloor) ? UP : DOWN;
    moveTimer.start(MOVE_DURATION);
}

void Elevator::processMove() {
    currentFloor += direction;
    display.update(currentFloor, direction);
    // If the elevator has reached the target floor, open the door
    if (currentFloor == queuedTargetFloors.front()) {
        queuedTargetFloors.pop();
        halt();
    } else {
        move();
    }
}

void Elevator::halt() {
    direction = IDLE;
    display.update(currentFloor, direction);
    // If the elevator is out of service, keep the door open and alert the passengers
    if (outOfService) {
        keepDoorOpen();
        display.alert(SAFEFLOOR);
        audioSystem.alert(SAFEFLOOR);
        while (numPassengers > 0) passengerExit(); // Let the passengers out
    } else {
        Coordinator::getInstance()->getFloorButtonPanel(currentFloor)->reset(); // Turn off the floor button
        openDoor();
    }
}

void Elevator::moveToSafeFloor() {
    // Stop the elevator and clear the queue
    moveTimer.stop();
    outOfService = true;
    queuedTargetFloors = std::queue<int>();
    queuedTargetFloors.push(Coordinator::getInstance()->getSafeFloor());
    // If the door is open, close it immediately (this will also call move())
    closeDoorImmediately();
}

void Elevator::openDoor() {
    if (!doorOpen) ring();
    doorOpen = true;
    display.update(currentFloor, direction);
    doorTimer.start(DEFAULT_COUNTDOWN);
    MainWindow* mainWindow = MainWindow::getMainWindow();
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " door open.");
}

void Elevator::closeDoor() {
    doorOpen = false;
    display.update(currentFloor, direction);
    if (doorSensor.update()) {
        handleObstacle();
        return;
    }
    if (weightSensor.update()) {
        handleOverload();
        return;
    }
    ring();
    MainWindow* mainWindow = MainWindow::getMainWindow();
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " door closed.");
    if (!queuedTargetFloors.empty()) {
        move();
    } else {
        Coordinator::getInstance()->update(true);
    }
}

void Elevator::closeDoorImmediately() {
    // Stop the door timer and close the door
    doorTimer.stop();
    closeDoor();
}

void Elevator::keepDoorOpen() {
    openDoor();
    doorTimer.stop();
}

void Elevator::handleObstacle() {
    if (!doorOpen) keepDoorOpen();

    QTimer::singleShot(SENSOR_TICK, this, [this]() {
        if (!doorSensor.update()) {
            closeDoorImmediately();
        } else if (doorSensor.isRepeatedlyTriggered()) {
            // If the sensor is repeatedly triggered, give alerts
            handleObstacle();
            display.alert(DOOROBSTACLE);
            audioSystem.alert(DOOROBSTACLE);
        } else {
            // If the sensor is still triggered, keep the door open by calling the function again
            handleObstacle();
        }
    });
}

void Elevator::handleOverload() {
    if (!doorOpen) keepDoorOpen();
    // The specs did not say if the alerts should only happen when the sensor is repeatedly triggered
    display.alert(OVERLOAD);
    audioSystem.alert(OVERLOAD);

    QTimer::singleShot(SENSOR_TICK, this, [this]() {
        if (!weightSensor.update()) {
            closeDoorImmediately();
        } else {
            // If the sensor is still triggered, keep the door open by calling the function again
            handleOverload();
        }
    });
}

void Elevator::passengerEnter() {
    numPassengers++;
    displayPassengerCount();
}

void Elevator::passengerExit() {
    // The elevator will not go below 0 passengers
    if (numPassengers > 0) numPassengers--;
    displayPassengerCount();
}

void Elevator::displayPassengerCount() const {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " now has " + std::to_string(numPassengers) + " passengers.");
}

void Elevator::handleFireAlarm() {
    moveToSafeFloor();
    display.alert(FIREALARM);
    audioSystem.alert(FIREALARM);
}

void Elevator::handlePowerOut() {
    switchToBattery();
    moveToSafeFloor();
    display.alert(POWEROUT);
    audioSystem.alert(POWEROUT);
}

void Elevator::switchToBattery() {
    usingBattery = true;
}

void Elevator::ring() {
    audioSystem.alert("Ding! Floor " + std::to_string(currentFloor) + ".");
}

void Elevator::handleHelpSignal() {
    audioSystem.alert(HELP);

    // Wait 5 seconds, the building safety does not respond in time, make an emergency call
    QTimer::singleShot(HELP_RESPONSE_TIME, this, [this]() {
        MainWindow* mainWindow = MainWindow::getMainWindow();
        bool buildingSafetyResponse = mainWindow->findChild<QCheckBox*>("toggleBuildingSafety")->isChecked();

        if (buildingSafetyResponse) audioSystem.buildingSafetyReply();
        else audioSystem.emergencyCall();
    });
}
