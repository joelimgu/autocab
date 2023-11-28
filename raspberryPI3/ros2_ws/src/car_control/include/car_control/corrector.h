#ifndef CORRECTOR_H
#define CORRECTOR_H

#include <stdint.h>
/* RPM is contained within [0,68], for both reverse and forward operation */
#define MAX_RPM 68.0

int piCorrector(float kp,float ki,float Te,float& smallPwmCmd,float& past_pwm_error,float& current_pwm_error);

float pwmError(float desiredRPM,float measuredRPM,float maxRPM);

#endif