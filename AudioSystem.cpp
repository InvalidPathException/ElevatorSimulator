#include "AudioSystem.h"

AudioSystem::AudioSystem(int elevatorID) : elevatorID(elevatorID) {}

void AudioSystem::alert(const std::string& message) const {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " audio system: " + message);
}

void AudioSystem::buildingSafetyReply() const {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    // The building safety replied
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " has called for help.");
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " received building safety reply");
}

void AudioSystem::emergencyCall() const {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    // The building safety did not respond, make an emergency call
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " has called for help, the building safety did not respond.");
    mainWindow->addEventLog("Elevator " + std::to_string(elevatorID) + " made emergency call");
}
