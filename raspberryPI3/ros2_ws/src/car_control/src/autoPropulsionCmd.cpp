#include "../include/car_control/autoPropulsionCmd.h"

int piCorrector(float kp, float ki, float Te, float& smallLeftRearPwmCmd,float& smallRightRearPwmCmd,float& past_pwm_error,float& current_pwm_error)
{
    /* Calculating current pwm commands based on previous pwm commands and errors, these are based off of a discrete time analysis of the PI corrector */
    smallLeftRearPwmCmd = smallLeftRearPwmCmd + (kp + 0.5*ki*Te)*current_pwm_error + (0.5*ki*Te - kp)*past_pwm_error;
    smallRightRearPwmCmd = smallLeftRearPwmCmd;
    past_pwm_error = current_pwm_error;
    return 0;
}

float pwmError(float desiredRPM,float measuredRPM,float maxRPM)
{
    float error = (desiredRPM - measuredRPM) * (50/maxRPM);
    return error;
}