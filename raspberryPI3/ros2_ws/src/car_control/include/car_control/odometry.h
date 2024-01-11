#ifndef ODOMETRY_H
#define ODOMETRY_H


namespace odom 
{
    int estimate_pos(float steeringAngle, 
                    float delta_t, 
                    float theta_current,
                    float theta_past,
                    float current_speed,
                    float past_posx,
                    float past_posy, 
                    float current_posx, 
                    float current_posy);

    /* Je dois changer le passage d'argument pour qu'il soit moins encombrant */

}


#endif