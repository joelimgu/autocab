#include "../include/car_control/obstacle_detection.h"

#include "std_msgs/msg/string.hpp"


/*
* Cette fonction permet de s'arreter lorsqu'un obstacle est détecter par
* les capteurs ultrasons (et revenir en arriere ?)
*/

int ObstacleCmdFront(int16_t front_left, int16_t front_center, int16_t front_right){

    if (front_left <= 50 || front_center <= 50 || front_right <= 50) {
        return STOP;
    }
    return 0;
}
int ObstacleCmdRear(int16_t rear_left, int16_t rear_center, int16_t rear_right) {
    

    if (rear_left <= 50 || rear_center <= 50 || rear_right <= 50) {
        return STOP;
    }
    return 0;
}

