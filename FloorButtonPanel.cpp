#include "FloorButtonPanel.h"

FloorButtonPanel::FloorButtonPanel(int floor) : floor(floor), upButtonIlluminated(false), downButtonIlluminated(false) {}

void FloorButtonPanel::pressButton(int direction) {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    if (direction == UP) {
        upButtonIlluminated = true;
        mainWindow->addEventLog("Up button illuminated on floor " + std::to_string(floor));
    } else {
        downButtonIlluminated = true;
        mainWindow->addEventLog("Down button illuminated on floor " + std::to_string(floor));
    }
    Coordinator::getInstance()->callFreeElevator(floor);
}

void FloorButtonPanel::reset() {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    if (upButtonIlluminated) {
        upButtonIlluminated = false;
        mainWindow->addEventLog("Up button extinguished on floor " + std::to_string(floor));
    }
    if (downButtonIlluminated) {
        downButtonIlluminated = false;
        mainWindow->addEventLog("Down button extinguished on floor " + std::to_string(floor));
    }
}
