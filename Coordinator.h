#ifndef COORDINATOR_H
#define COORDINATOR_H
#include "defs.h"

class Elevator; // Forward declaration to avoid circular dependency
class FloorButtonPanel; // Forward declaration to avoid circular dependency

class Coordinator {
public:
    ~Coordinator();
    static Coordinator* getInstance();
    static Coordinator* newInstance();
    void callFreeElevator(int floor);
    void fireAlarm();
    void powerOut();
    void update(bool isFree);
    int getSafeFloor() const { return safeFloor; }
    Elevator* getElevator(int i) const { return elevators[i]; }
    FloorButtonPanel* getFloorButtonPanel(int i) const { return floorButtonPanels[i]; }
    int getNumFloors() const { return numFloors; }
    static Coordinator* singleton;

private:
    Coordinator(int numFloors, int numElevators, int safeFloor);
    int numFloors;
    int numElevators;
    int safeFloor;
    std::queue<int> queuedRequests;
    FloorButtonPanel** floorButtonPanels;
    Elevator** elevators;
};

#endif //COORDINATOR_H
