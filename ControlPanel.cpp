#include "ControlPanel.h"
#include "Elevator.h"

ControlPanel::ControlPanel(Elevator* elevator) : elevator(elevator) {}

void ControlPanel::chooseDestination(int floor) const {
    elevator->addTargetFloor(floor);
}

void ControlPanel::pressHelpButton() const {
    elevator->handleHelpSignal();
}

void ControlPanel::pressOpenButton() const {
    elevator->keepDoorOpen();
}

void ControlPanel::releaseOpenButton() const {
    elevator->openDoor();
}

void ControlPanel::pressCloseButton() const {
    elevator->closeDoorImmediately();
}
