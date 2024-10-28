#include "DisplayScreen.h"

DisplayScreen::DisplayScreen(int elevatorID) : elevatorID(elevatorID) {}

void DisplayScreen::alert(const std::string& message) const {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " display: " + message);
}

void DisplayScreen::update(int floor, int direction) const {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    mainWindow->updateDisplay(elevatorID, floor, direction);
}
