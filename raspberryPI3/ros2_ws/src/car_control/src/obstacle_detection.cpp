#include "../include/car_control/obstacle_detection.h"

#include "std_msgs/msg/string.hpp"


/*
* Cette fonction permet de s'arreter lorsqu'un obstacle est détecter par
* les capteurs ultrasons (et revenir en arriere ?)
*/

int ObstacleCmd(int16_t front_left, int16_t front_center, int16_t front_right, int16_t rear_left, int16_t rear_center, int16_t rear_right){

    if (front_left <= 50) {
        return STOP;
    }
    
    if (front_center <= 50) {
        return STOP;
    } 

    if (front_right <= 50) {
        return STOP;
    }
    
    if (rear_left <= 50) {
        return STOP;
    }
    
    if (rear_center <= 50) {
        return STOP;
    }
    
    if (rear_right <= 50) {
        return STOP;
    } else return MAX_PWM_RIGHT;
    
    
}

