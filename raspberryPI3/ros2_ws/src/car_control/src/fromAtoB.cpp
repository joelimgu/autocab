#include "../include/car_control/fromAtoB.h"

/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination)
Pour l'instant, ce point B est fixé à la main dans le code 
*/

int * straightLine(float aLatitude, float aLongitude, float[2] aVector, float& requestedThrottle, bool& reverse, float& requestedAngle){

    float bVector[2];

    float bLatitude = 43.570596;
    float bLongitude = 1.466500;

    //Cette section permet de réduire la vitesse proche du point B
    float distance = sqrt(pow((bLatitude - aLatitude), 2) + pow((bLongitude - aLongitude), 2));

    requestedThrottle = distance * 0.1;
    if requestedThrottle > 1.0{
        requestedThrottle = 1.0;
    }

    //Cette section permet de déterminer si la voiture doit aller à droite ou gauche

    bVector[0] = bLatitude - aLatitude ;
    bVector[1] = bLongitude - aLongitude ;

    float angle = acos((aVector[0]*bVector[0] + aVector[1]*bVector[1])/(sqrt(pow(aVector[0], 2) + pow(aVector[1], 2)) * sqrt(pow(bVector[0], 2) + pow(bVector[1], 2))));
     //calculer vrai angle
    if angle > 0.1{
        requestedAngle = 1.0;
    } else if angle < -0.1{
        requestedAngle = -1.0;
    } else {
        requestedAngle = angle;
    }


    return 0;

    
}