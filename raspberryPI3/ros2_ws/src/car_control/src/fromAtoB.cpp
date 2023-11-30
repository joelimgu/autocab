#include "../include/car_control/fromAtoB.h"

/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination)
Pour l'instant, ce point B est fixé à la main dans le code 
*/

int * straightLine(float aLatitude, float aLongitude, float[2] aVector, float& requestedThrottle, bool& reverse, float& requestedAngle){

    float bVector[2];
    float bVectorOrtho[2];

    float bLatitude = 43.570596;
    float bLongitude = 1.466500;

    //Cette section permet de réduire la vitesse proche du point B
    float distance = sqrt(pow((bLatitude - aLatitude), 2) + pow((bLongitude - aLongitude), 2));

    if (distance > MIN_DISTANCE_FOR_MAX_THROTTLE){
        requestedThrottle = 1.0;
    }else{
        requestedThrottle = 1.0 * distance / MIN_DISTANCE_FOR_MAX_THROTTLE;
    }

    if(requestedThrottle < 0.1){
        requestedThrottle = 0;
        reverse = false;
        requestedAngle = 0;
    }else{

        //Cette section permet de déterminer si la voiture doit aller à droite ou gauche

        bVector[0] = bLatitude - aLatitude ;
        bVector[1] = bLongitude - aLongitude ;
        bvectorOrtho[0] = -bVector[1];
        bvectorOrtho[1] = bVector[0];


        float angle = degrees(acos((aVector[0]*bVector[0] + aVector[1]*bVector[1])/(sqrt(pow(aVector[0], 2) + pow(aVector[1], 2)) * sqrt(pow(bVector[0], 2) + pow(bVector[1], 2)))));
        
        if (angle > MIN_ANGLE_FOR_MAX_STEERING){
            if (aVector[0]*bVectorOrtho[0] + aVector[1]*bVectorOrtho[1] > 0){
                requestedAngle = -1.0;
            }else{
                requestedAngle = 1.0;
            }
        }else{
            requestedAngle = 0;
        }

        reverse = false;

    }

    return 0;

    
}