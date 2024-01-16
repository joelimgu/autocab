#ifndef ODOMETRY_H
#define ODOMETRY_H


namespace odom 
{
    /* Created a vector containing positional data in meters for x axis and y axis */
    int initialise_position(float vector[2])
    int odom::estimate_pos(float delta_t,
                    float L,
                    float past_steeringAngle,
                    float past_theta,
                    float &past_speed,
                    float past_posx,
                    float past_posy, 
                    float &current_posx, 
                    float &current_posy,
                    float current_theta)
    /* Je dois changer le passage d'argument pour qu'il soit moins encombrant */
}


#endif