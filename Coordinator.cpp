#include "Coordinator.h"
#include "Elevator.h"
#include "FloorButtonPanel.h"

Coordinator* Coordinator::singleton = nullptr;

Coordinator::Coordinator(int numFloors, int numElevators, int safeFloor) : numFloors(numFloors), numElevators(numElevators), safeFloor(safeFloor) {
    floorButtonPanels = new FloorButtonPanel*[numFloors];
    for (int i = 0; i < numFloors; i++) {
        floorButtonPanels[i] = new FloorButtonPanel(i);
    }
    elevators = new Elevator*[numElevators];
    for (int i = 0; i < numElevators; i++) {
        elevators[i] = new Elevator(i);
    }
}

Coordinator::~Coordinator() {
    // This will delete all other objects we have ever constructed
    for (int i = 0; i < numFloors; i++) {
        delete floorButtonPanels[i];
    }
    for (int i = 0; i < numElevators; i++) {
        delete elevators[i];
    }
    delete[] floorButtonPanels;
    delete[] elevators;
    singleton = nullptr;
}

Coordinator* Coordinator::getInstance() {
    return singleton; // Not the standard Singleton getter because Qt does not work well with it, and we need to be able to restart the program
}

Coordinator* Coordinator::newInstance() {
    // This is the "getter" that creates the instance, but we only call it when (re)starting the simulation
    if (singleton != nullptr) {
        delete singleton;
    }
    MainWindow* mainWindow = MainWindow::getMainWindow();
    singleton = new Coordinator(mainWindow->getNumFloors(), mainWindow->getNumElevators(), mainWindow->getSafeFloor());
    return singleton;
}

void Coordinator::callFreeElevator(int floor) {
    for (int i = 0; i < numElevators; i++) {
        if (elevators[i]->isIdle()) {
            if (elevators[i]->addTargetFloor(floor)) { // The elevator can refuse to take the request
                elevators[i]->move();
                return;
            }
        }
    }
    queuedRequests.push(floor);
}

void Coordinator::fireAlarm() {
    // Simply tell every elevator to go to the safe floor
    for (int i = 0; i < numElevators; i++) {
        elevators[i]->handleFireAlarm();
    }
}

void Coordinator::powerOut() {
    // Simply tell every elevator to go to the safe floor
    for (int i = 0; i < numElevators; i++) {
        elevators[i]->handlePowerOut();
    }
}

void Coordinator::update(bool isFree) {
    // If an elevator is free and there are queued requests, call it
    if (isFree && !queuedRequests.empty()) {
        callFreeElevator(queuedRequests.front());
        queuedRequests.pop();
    }
}
