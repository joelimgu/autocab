#ifndef __car_control_node_H
#define __car_control_node_H

#include <stdint.h>
#include <string.h>  


#define PERIOD_UPDATE_CMD 5ms //Period to update proupulsion and steering command in [ms]

#define STOP 50 //PWM value to stop motors

#define MIN_UPDATE_COORDINATES 0.00001 //Minimum distance to update coordinates in [deg]



#endif /*__ car_control_node_H */