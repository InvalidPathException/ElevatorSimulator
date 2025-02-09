#ifndef SENSOR_H
#define SENSOR_H
#include "mainwindow.h"
#include <QApplication>
#include <QCheckBox>

class Sensor {
public:
    Sensor() = default;
    virtual ~Sensor() = default;
    virtual bool update() = 0;
};

#endif //SENSOR_H
