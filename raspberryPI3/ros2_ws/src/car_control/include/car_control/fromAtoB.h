#ifndef __fromeAtoB_H
#define __fromeAtoB_H

#include <cstdint>
#include <stdint.h>


/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination)
Pour l'instant, ce point B est fixé à la main dans le code 
*/
int * straightLine(float aLatitude, float aLongitude, vector<float> aVector, float& requestedThrottle, bool& reverse);

#endif /*__fromeAtoB_H */