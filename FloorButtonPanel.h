#ifndef FLOORBUTTONPANEL_H
#define FLOORBUTTONPANEL_H
#include "mainwindow.h"
#include "Coordinator.h"
#include "defs.h"

class FloorButtonPanel {
public:
    explicit FloorButtonPanel(int floor); // coordinator will be initialized using getInstance()
    void pressButton(int direction);
    void reset();

private:
    int floor;
    bool upButtonIlluminated;
    bool downButtonIlluminated;
};

#endif //FLOORBUTTONPANEL_H
