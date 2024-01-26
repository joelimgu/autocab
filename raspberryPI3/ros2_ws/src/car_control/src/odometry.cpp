#include "../include/car_control/odometry.h"
#include <math.h>


int odom::initialise_position(bool &past_reverse,float past_position[2],float current_position[2],float past_speeds[2],
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
                    float current_position[2])
{
    float x_dot, y_dot;
    float theta_dot;
    int i = 0;
    float angle_coeff = -(41/0.97);
    past_steeringAngle = past_steeringAngle*(M_PI*2/360)*angle_coeff;

    if (past_steeringAngle <= 0) 
    {
        i = 0;
    }
    else if (past_steeringAngle > 0)
    {
        i = 1;
    }

    /* Empêcher des enregistrements de valeurs sporadiques !!! Un filtre ? Augmenter la fréquence de mise à jour ? */

    /* Calculating theta_dot and speeds */
    theta_dot = past_speeds[i]*std::tan(past_steeringAngle)/L; //The choice of the wheel that should be used is parametrized by i
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
    if (reverse)
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

/* Used to filter speed values from motors_feedback */
void odom::lowPassFilter(float tau, float T, float &past_input, float &current_input, float &past_output, float &current_output)
{
    current_output = (current_input * T + past_input * T + (2*tau -T)*past_output) * (1/(T + 2 * tau));
    past_output = current_output;
    past_input = current_input;
}

/* Essential variables to convert coordinates */

double K0 = 0.9996;
double E = 0.00669438;
double E2 = E * E;
double E3 = E2 * E;
double E_P2 = E / (1.0 - E);
double SQRT_E = std::sqrt(1 - E);
double _E = (1 - SQRT_E) / (1 + SQRT_E);
double _E2 = _E * _E;
double _E3 = _E2 * _E;
double _E4 = _E3 * _E;
double _E5 = _E4 * _E;
double M1 = (1 - E / 4 - 3 * E2 / 64 - 5 * E3 / 256);
double M2 = (3 * E / 8 + 3 * E2 / 32 + 45 * E3 / 1024);
double M3 = (15 * E2 / 256 + 45 * E3 / 1024);
double M4 = (35 * E3 / 3072);
double P2 = (3. / 2 * _E - 27. / 32 * _E3 + 269. / 512 * _E5);
double P3 = (21. / 16 * _E2 - 55. / 32 * _E4);
double P4 = (151. / 96 * _E3 - 417. / 128 * _E5);
double P5 = (1097. / 512 * _E4);
double R = 6378137.0;

int odom::zone_number_to_central_longitude(int zone_number)
{
    return ((zone_number - 1) * 6 - 180 + 3);
}

double odom::mod_angle(double value)
{
    /* Returns angle in radians to be between -pi and pi */
    return fmod((value + M_PI),(2 * M_PI)) - M_PI;
}

void odom::to_latlon(double easting,double northing,int zone_number,char zone_letter,bool northern,double latlon_tab[2])
{   
    northern = (zone_letter >= 'N');

    double x = easting - 500000;
    double y = northing;

    if (!northern)
    {
        y -= 10000000;
    }

    double m = y / K0;
    double mu = m / (R * M1);

    double p_rad = (mu +
             P2 * std::sin(2 * mu) +
             P3 * std::sin(4 * mu) +
             P4 * std::sin(6 * mu) +
             P5 * std::sin(8 * mu));

    double p_sin = std::sin(p_rad);
    double p_sin2 = p_sin * p_sin;

    double p_cos = std::cos(p_rad);

    double p_tan = p_sin / p_cos;
    double p_tan2 = p_tan * p_tan;
    double p_tan4 = p_tan2 * p_tan2;
    
    double ep_sin = 1 - E * p_sin2;
    double ep_sin_sqrt = std::sqrt(1 - E * p_sin2);

    double n = R / ep_sin_sqrt;
    double r = (1 - E) / ep_sin;

    double c = E_P2 * p_cos*p_cos;
    double c2 = c * c;

    double d = x / (n * K0);
    double d2 = d * d;
    double d3 = d2 * d;
    double d4 = d3 * d;
    double d5 = d4 * d;
    double d6 = d5 * d;

    double latitude = (p_rad - (p_tan / r) *
                (d2 / 2 -
                 d4 / 24 * (5 + 3 * p_tan2 + 10 * c - 4 * c2 - 9 * E_P2)) +
                 d6 / 720 * (61 + 90 * p_tan2 + 298 * c + 45 * p_tan4 - 252 * E_P2 - 3 * c2));

    double longitude = (d -
                 d3 / 6 * (1 + 2 * p_tan2 + c) +
                 d5 / 120 * (5 - 2 * c + 28 * p_tan2 - 3 * c2 + 8 * E_P2 + 24 * p_tan4)) / p_cos;

    longitude = mod_angle(longitude + (M_PI/180)*(zone_number_to_central_longitude(zone_number)));

    latlon_tab[0] = (180/M_PI)*latitude;
    latlon_tab[1] = (180/M_PI)*longitude;
}