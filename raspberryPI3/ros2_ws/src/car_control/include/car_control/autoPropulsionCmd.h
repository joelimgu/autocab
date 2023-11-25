#ifndef AUTO_PROPULSION_CMD_H
#define AUTO_PROPULSION_CMD_H

#include <stdint.h>
#define MAX_RPM 100.0
int piCorrector(float kp,float ki,float Te,uint8_t& leftRearPwmCmd,uint8_t& rightRearPwmCmd,float past_pwm_error,float current_pwm_error);

uint8_t pwmError(float desiredRPM,float measuredRPM,float maxRPM);

#endif