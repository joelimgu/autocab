#ifndef __fromAtoB_H
#define __fromAtoB_H

#include <cstdint>
#include <stdint.h>

#define MIN_ANGLE_FOR_MAX_STEERING 20 //Minimum angle to set maximum steering in [deg]
#define MIN_DISTANCE_FOR_MAX_THROTTLE 0.5 //Minimum distance to set maximum throttle in [m]

/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination)
Pour l'instant, ce point B est fixé à la main dans le code 
*/
int * straightLine(float aLatitude, float aLongitude, float aVector[2], float& requestedThrottle, bool& reverse, float& requestedAngle);

#endif /*__fromAtoB_H */