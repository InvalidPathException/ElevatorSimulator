#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H
#include "Sensor.h"

class WeightSensor final : public Sensor {
public:
    WeightSensor();
    bool update() override;
};

#endif //WEIGHTSENSOR_H
