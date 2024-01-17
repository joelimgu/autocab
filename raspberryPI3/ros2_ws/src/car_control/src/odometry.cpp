#include "../include/car_control/odometry.h"
#include <math.h>


int odom::initialise_position(float past_reverse,float past_position[2],float current_position[2],float past_speeds[2],
                    float &past_theta,
                    float &current_theta) /* MISRA-C suggests using [2]*/
{
    current_position[0] = 0.0;
    current_position[1] = 0.0;
    past_position[0] = 0.0;
    past_position[1] = 0.0;
    past_speeds[0] = 0.0;
    past_speeds[1] = 0.0;
    past_theta = 0.0;
    current_theta = 0.0;
    past_reverse = false;
    return 0;
}

int odom::estimate_pos(float delta_t,
                    float L,
                    bool past_reverse,
                    bool reverse,
                    float wheel_radius,
                    float &past_steeringAngle,
                    float &past_theta,
                    float &current_theta,
                    float past_speeds[2],
                    const float &left_rear_RPM,
                    const float &right_rear_RPM,
                    float past_position[2], 
                    float current_position[2])
{
    float x_dot, y_dot;
    float theta_dot;
    int i;

    if (past_steeringAngle < 0) 
    {
        i = 0;
    }
    else if (past_steeringAngle > 0)
    {
        i = 1;
    }

    /* Empêcher des enregistrements de valeurs sporadiques !!! Un filtre ? Augmenter la fréquence de mise à jour ? */

    /* Calculating theta_dot and speeds */
    theta_dot = past_speeds[i]/(L/std::tan(past_steeringAngle)); //The choice of the wheel that should be used is parametrized by i
    current_theta = past_theta + theta_dot * delta_t;
    x_dot = past_speeds[i] * (-std::sin(past_theta)); // the speed can be either negative or positive, and either right or left wheel speed
    y_dot = past_speeds[i] * std::cos(past_theta);
    
    /* Computing estimated current positions in the fixed start reference frame */
    current_position[0] = past_position[0] + x_dot * delta_t;
    current_position[1] = past_position[1] + y_dot * delta_t;

    /* Setting past values to the current ones for later */
    past_position[0] = current_position[0];
    past_position[1] = current_position[1];
    past_theta = current_theta;
    past_reverse = reverse;
    
    /* Updating speed values for both right and left rear wheels */
    if (past_reverse)
    {
        past_speeds[0] = -(wheel_radius/30)*M_PI*right_rear_RPM;
        past_speeds[1] = -(wheel_radius/30)*M_PI*left_rear_RPM;
    }
    else
    {
        past_speeds[0] = (wheel_radius/30)*M_PI*right_rear_RPM;
        past_speeds[1] = (wheel_radius/30)*M_PI*left_rear_RPM;
    }
    return 0;
}