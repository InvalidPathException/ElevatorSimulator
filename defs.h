#ifndef DEFS_H
#define DEFS_H

#include <iostream>
#include <string>
#include <queue>
#include <unistd.h>

#define UP 1
#define DOWN -1
#define IDLE 0
#define SENSOR_THRESHOLD 10
#define HELP_RESPONSE_TIME 5000 // 5 seconds to respond to help button
#define DEFAULT_COUNTDOWN 10000 // 10 seconds for the door to close
#define MOVE_DURATION 2000 // 2 seconds to move one floor
#define SENSOR_TICK 1000 // 1 second to check sensors
#define DOOROBSTACLE "Obstacle detected, please remove it."
#define OVERLOAD "Overload detected, door will remain open."
#define SAFEFLOOR "You are now evacuated to a safe floor, please leave the elevator immediately."
#define FIREALARM "Fire alarm detected, moving to safe floor."
#define POWEROUT "Power outage detected, you are on backup battery and moving to safe floor."
#define HELP "Help button pressed, please wait for assistance."

#endif //DEFS_H
