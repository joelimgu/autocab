#include "../include/car_control/autoPropulsionCmd.h"

int piCorrector(float kp, float ki, float Te, uint8_t& leftRearPwmCmd,uint8_t& rightRearPwmCmd,uint8_t past_pwm_error,uint8_t current_pwm_error)
{
    /* Calculating current pwm commands based on previous pwm commands and errors, these are based off of a discrete time analysis of the PI corrector */
    leftRearPwmCmd = leftRearPwmCmd + (kp + 0.5*ki*Te)*current_pwm_error + (0.5*ki*Te - kp)*past_pwm_error;
    rightRearPwmCmd = leftRearPwmCmd;
    return 0;
}

uint8_t pwmError(float desiredRPM,float measuredRPM,float maxRPM)
{
    uint8_t error = (desiredRPM - measuredRPM) * (50/maxRPM);
    return error;
}