#include "../include/car_control/odometry.h"
#include <math>

int odom::estimate_pos(float steeringAngle, 
                    float delta_t, 
                    float current_theta,
                    float past_theta,
                    float current_speed,
                    float past_posx,
                    float past_posy, 
                    float current_posx, 
                    float current_posy,
                    float L)
{
    float delta_theta = (current_speed * std::tan(steeringAngle))/L;
    current_theta = past_theta + delta_theta * delta_t;
    float delta_x, delta_y;

    /* Approximations grossières */
    delta_x = current_speed * std::cos(current_theta); 
    delta_y = -current_speed * std::sin(current_theta);

    current_posx = past_posx + delta_x * delta_t;
    current_posy = past_posy + delta_y * delta_t;
    
    return 0;
}