#ifndef __propulsionCmd_H
#define __propulsionCmd_H

#include <cstdint>
#include <stdint.h>


/* Calculate rightRearPwmCmd and leftRearPwmCmd (PWM) in MANUAL mode (from joystick orders)
*
* The joystick sends throttle order, which is directly transformed into PWM. The PWMs are equal for both motors in Manual Mode
* 
*/
int * manualPropulsionCmd(float requestedThrottle, bool reverse, float& leftRearPwmCmd, float& rightRearPwmCmd);

#endif /*__ propulsionCmd_H */