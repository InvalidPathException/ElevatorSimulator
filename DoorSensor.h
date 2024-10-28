#ifndef DOORSENSOR_H
#define DOORSENSOR_H
#include "Sensor.h"

class DoorSensor final : public Sensor {
public:
    DoorSensor();
    bool update() override;
    bool isRepeatedlyTriggered() const;

private:
    int timesTriggered;
};

#endif //DOORSENSOR_H
