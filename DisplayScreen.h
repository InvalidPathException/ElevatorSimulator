#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H
#include "defs.h"
#include "mainwindow.h"
#include <QApplication>

class DisplayScreen {
public:
    explicit DisplayScreen(int elevatorID);
    void alert(const std::string& message) const;
    void update(int floor, int direction) const;
private:
    int elevatorID;
};

#endif //DISPLAYSCREEN_H
