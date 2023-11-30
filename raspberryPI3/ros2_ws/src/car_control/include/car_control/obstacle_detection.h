#ifndef __obstacle_detection_H
#define __obstacle_detection_H

#include <stdint.h>
#include <string.h>  

#define STOP 50 //PWM value to stop motors
#define MAX_PWM_LEFT 0 //PWM value to go forward
#define MAX_PWM_RIGHT 100 //PWM value to go backward

/*
* Cette fonction permet de s'arreter lorsqu'un obstacle est détecter par
* les capteurs ultrasons (et revenir en arriere ?)
*/

int ObstacleCmd(int16_t front_left, int16_t front_center, int16_t front_right, int16_t rear_left, int16_t rear_center, int16_t rear_right);


#endif /*__ obstacle_detection_H */