#include "../include/car_control/odometry.h"
#include <math.h>


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
    return 0;
}

int odom::estimate_pos(float delta_t,
                    float L,
                    bool reverse,
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
    float true_speed;
    if(reverse)
    {
        true_speed = -past_speed;
        current_speed = -((wheel_radius/30)*M_PI)*((left_rear_RPM + right_rear_RPM)/2); /* Valeur moyenne des RPM */
        past_speed = current_speed;
    }
    else
    {
        true_speed = past_speed;
        current_speed = ((wheel_radius/30)*M_PI)*((left_rear_RPM + right_rear_RPM)/2);
        past_speed = current_speed;
    }
    float delta_x, delta_y;
    float theta_dot = true_speed/((L/std::tan(past_steeringAngle)) + 0.25); /* Peut etre rajouter un truc qui change la procedure pour chaque cas et qui empeche des enregistrements de valeurs sporadiques !!!*/
    current_theta = past_theta + theta_dot * delta_t;
    past_theta = current_theta;

    /* Approximations grossières */
    delta_x = true_speed * std::cos(past_theta); 
    delta_y = -true_speed * std::sin(past_theta);
    current_position[0] = past_position[0] + delta_x * delta_t;
    current_position[1] = past_position[1] + delta_y * delta_t;
    past_position[0] = current_position[0];
    past_position[1] = current_position[1];
    return 0;
}