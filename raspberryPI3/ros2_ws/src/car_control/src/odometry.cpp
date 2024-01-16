#include "../include/car_control/odometry.h"
#include <math>


int initialise_position(float vector[2]) /* According to MISRA-C */
{
    vector[0] = 0.0;
    vector[1] = 0.0;
    return 0
}

int odom::estimate_pos(float delta_t,
                    float L,
                    float &past_steeringAngle,
                    float &past_theta,
                    float &current_theta,
                    float &past_speed,
                    float past_position[2], 
                    float current_position[2])
{
    float theta_dot = (past_speed * std::tan(past_steeringAngle))/L;
    current_theta = past_theta + theta_dot * delta_t;
    float delta_x, delta_y;

    /* Approximations grossières */
    delta_x = past_speed * std::cos(past_theta); 
    delta_y = -past_speed * std::sin(past_theta);

    current_position[0] = past_position[0] + delta_x * delta_t;
    current_position[1] = past_position[1] + delta_y * delta_t;
    
    return 0;
}