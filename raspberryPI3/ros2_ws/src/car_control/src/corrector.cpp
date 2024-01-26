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

float trajAngleError(float desiredAngle,float measuredAngle)
{
    float error = (desiredAngle - measuredAngle);
    return error;
}
int correctTrajAngle(float& steeringPwmCmd, float& past_angle_error,float& current_angle_error,float& steeringAngle)
{
    /* No need for the same if condition with reverse since the output goes from negative to positive this time */
    float smallPwmCmd;
    smallPwmCmd = steeringPwmCmd - 50;
    currentAngleError = trajAngleError(0,steeringAngle); /* Multiply this by a coefficient to get the usual steeringPWM, though usually the steering works with All or Nothing control, desired angle is 0 */
    piCorrector(0.12,1.22,0.001,smallPwmCmd,past_angle_error,current_angle_error);
    return 0;
}

/* reverse = 1 when we want the wheel to reverse, otherwise reverse = 0*/
int correctWheelSpeed(float& PwmCmd, float& past_pwm_error,float& current_pwm_error,float& wheelRPM,bool reverse)
{
    /* Assumptions made, such as both Wheels' speeds are the same at all times */
    /* Closed loop error calculation */
    float smallPwmCmd;
    current_pwm_error = pwmError(20.0,wheelRPM,MAX_RPM);
    if(reverse)
    {
        smallPwmCmd = 50 - PwmCmd;
    }
    else
    {
        smallPwmCmd = PwmCmd - 50;
    }
    piCorrector(0.12,1.22,0.001,smallPwmCmd,past_pwm_error,current_pwm_error);
    if(reverse)
    {
        PwmCmd = 50 - smallPwmCmd;
    }
    else
    {
        PwmCmd = smallPwmCmd + 50;
    }
    if (PwmCmd > 100)
    {
        /* Capping the PWM command to 100 */
        PwmCmd = 100;
    }
    else if (PwmCmd < 0)
    {
        PwmCmd = 0;
    }
    return 0;
}
