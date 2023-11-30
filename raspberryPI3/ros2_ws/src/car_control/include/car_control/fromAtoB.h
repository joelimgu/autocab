#ifndef __fromAtoB_H
#define __fromAtoB_H

#include <cstdint>
#include <stdint.h>


/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination)
Pour l'instant, ce point B est fixé à la main dans le code 
*/
int * straightLine(float aLatitude, float aLongitude, float[2] aVector, float& requestedThrottle, bool& reverse, float& requestedAngle);

#endif /*__fromAtoB_H */