#include "../include/car_control/corrector.h"

int piCorrector(float kp, float ki, float Te,float& smallPwmCmd, float& past_pwm_error,float& current_pwm_error)
{
    /* Calculating current pwm commands based on previous pwm commands and errors, these are based off of a discrete time analysis of the PI corrector */
    smallPwmCmd = smallPwmCmd + (kp + 0.5*ki*Te)*current_pwm_error + (0.5*ki*Te - kp)*past_pwm_error;
    past_pwm_error = current_pwm_error;
    return 0;
}

float pwmError(float desiredRPM,float measuredRPM,float maxRPM)
{
    float error = (desiredRPM - measuredRPM) * (50/maxRPM);
    return error;
}