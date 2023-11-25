#ifndef AUTO_PROPULSION_CMD_H
#define AUTO_PROPULSION_CMD_H

#include <stdint.h>

int piCorrector(float kp,float ki,float Te,uint8_t& leftRearPwmCmd,uint8_t& rightRearPwmCmd,uint8_t past_pwm_error,uint8_t current_pwm_error);

uint8_t pwmError(float desiredRPM,float measuredRPM,float maxRPM);

#endif