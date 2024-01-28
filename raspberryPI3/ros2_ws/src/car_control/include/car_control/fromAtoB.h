#ifndef __fromAtoB_H
#define __fromAtoB_H

#include <cstdint>
#include <stdint.h>
#include "rclcpp/rclcpp.hpp"
#include <unordered_map>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>



#define MIN_ANGLE_FOR_MAX_STEERING 15 //Minimum angle to set maximum steering in [deg]

#define MIN_DISTANCE_FOR_MAX_THROTTLE 3 //Minimum distance to set maximum throttle in [m]

#define EARTH_RADIUS 6367444.65 //Earth radius in [m]

#define MIN_THROTTLE 0.4

using namespace std;
/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination)
Pour l'instant, ce point B est fixé à la main dans le code 
*/
//bool straightLine(float aLatitude, float aLongitude, float bLatitude, float bLongitude, float aVector[2], float& requestedThrottle, bool& reverse, float& requestedAngle, rclcpp::Logger logger);
bool straightLine(float aLatitude, float aLongitude, float bLatitude, float bLongitude, float aVector[2], float& requestedThrottle, bool& reverse, float& requestedAngle, rclcpp::Logger logger,uint8_t& steeringPwmCmd,float& traj_past_angle_error,float& traj_current_angle_error);

/* Fonction qui permet de trouver le point le plus proche qui est accessible en ligne droite */
char detectClosestPoint(float currentLat, float currentLon, std::map<char, float[2]> &pointMap);

/* Calcule une distance avec les latitudes et longitudes de deux points */
float distance(float a[2], float b[2]);

/**
     * Represents a graph data structure.
     * 
     * A graph is a collection of nodes (vertices) and edges that connect these nodes.
     * It can be used to represent relationships between objects or entities.
**/
class Graph{
    
    std::unordered_map<char, const std::unordered_map<char, float>> vertices;
    
public:

    void add_vertex(char name, const std::unordered_map<char, float>& edges);
    std::vector<char> shortest_path(char start, char finish);
    void createGraph(std::map<char, float[2]> coordinates);
};


#endif /*__fromAtoB_H */