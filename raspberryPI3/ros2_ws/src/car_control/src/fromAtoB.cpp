#include "../include/car_control/fromAtoB.h"

/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination)
Pour l'instant, ce point B est fixé à la main dans le code 
*/

int * straightLine(float aLatitude, float aLongitude, vector<float> aVector, float& requestedThrottle, bool& reverse){

    if (aVector == {0,0}){
        return 0;
    } else {
        float bLatitude = 43.570596;
        float bLongitude = 1.466500;

        float distance = sqrt(pow((bLatitude - aLatitude), 2) + pow((bLongitude - aLongitude), 2));

        requestedThrottle = distance * 0.1;
        if requestedThrottle > 1.0{
            requestedThrottle = 1.0;
        }

        vector<float> bVector = {bLatitude - aLatitude, bLongitude - aLongitude};



        return 0;
    }

    
}