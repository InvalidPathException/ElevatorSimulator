#include "DoorSensor.h"

DoorSensor::DoorSensor() : Sensor(), timesTriggered(0) {}

bool DoorSensor::update() {
    MainWindow* mainWindow = MainWindow::getMainWindow();
    bool isTriggered = mainWindow->QObject::findChild<QCheckBox*>("testObstacle")->isChecked();
    if (isTriggered) {
        timesTriggered++;
        if (!isRepeatedlyTriggered()) {
            mainWindow->addEventLog("System: Door sensor is triggered"); // Log the event because otherwise we can't know what's happening
        }
    } else {
        timesTriggered = 0;
    }
    return isTriggered;
}

bool DoorSensor::isRepeatedlyTriggered() const {
    return timesTriggered > SENSOR_THRESHOLD;
}
