#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H
#include "defs.h"
#include "mainwindow.h"
#include <QApplication>

class AudioSystem {
public:
    explicit AudioSystem(int elevatorID);
    void alert(const std::string& message) const;
    void buildingSafetyReply() const;
    void emergencyCall() const;
private:
    int elevatorID;
};

#endif //AUDIOSYSTEM_H
