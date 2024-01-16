#include "../include/car_control/odometry.h"
#include <math>


int odom::initialise_position(float past_position[2],float current_position[2],float &past_speed,float &current_speed,
                    float &past_theta,
                    float &current_theta) /* MISRA-C suggests using [2]*/
{
    current_position[0] = 0.0;
    current_position[1] = 0.0;
    past_position[0] = 0.0;
    past_position[1] = 0.0;
    past_speed = 0;
    current_speed = 0;
    past_theta = 0;
    current_theta = 0;
    return 0
}

int odom::estimate_pos(float delta_t,
                    float L,
                    float wheel_radius,
                    float &past_steeringAngle,
                    float &past_theta,
                    float &current_theta,
                    float &past_speed,
                    float &current_speed,
                    const float &left_rear_RPM,
                    const float &right_rear_RPM,
                    float past_position[2], 
                    float current_position[2])
{
    float theta_dot = (past_speed * std::tan(past_steeringAngle))/L;
    current_theta = past_theta + theta_dot * delta_t;
    past_theta = current_theta;
    float delta_x, delta_y;

    /* Approximations grossières */
    delta_x = past_speed * std::cos(past_theta); 
    delta_y = -past_speed * std::sin(past_theta);

    current_position[0] = past_position[0] + delta_x * delta_t;
    current_position[1] = past_position[1] + delta_y * delta_t;
    past_position[0] = current_position[0];
    past_position[1] = current_position[1];
    current_speed = ((wheel_radius/30)*std::pi)*((left_rear_RPM + right_rear_RPM)/2); /* Valeur moyenne des RPM */
    past_speed = current_speed;
    return 0;
}