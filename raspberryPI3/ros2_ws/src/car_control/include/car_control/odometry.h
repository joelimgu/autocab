#ifndef ODOMETRY_H
#define ODOMETRY_H


namespace odom 
{
    /* Created a vector containing positional data in meters for x axis and y axis */
    int initialise_position(bool &past_reverse,float past_position[2],float current_position[2],float past_speeds[2],
                    float &past_theta,
                    float &current_theta);

    int estimate_pos(float delta_t,
                    float L,
                    bool &past_reverse,
                    bool reverse,
                    float wheel_radius,
                    float &past_steeringAngle,
                    float &past_theta,
                    float &current_theta,
                    float past_speeds[2],
                    const float &left_rear_RPM,
                    const float &right_rear_RPM,
                    float past_position[2], 
                    float current_position[2]);
    /* Je dois changer le passage d'argument pour qu'il soit moins encombrant */
}


#endif