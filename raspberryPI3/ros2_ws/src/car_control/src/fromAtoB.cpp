#include "../include/car_control/fromAtoB.h"
#include <math.h>
#include <stdio.h>
#include <unordered_map>
#include <map.h>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>


/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination)
Pour l'instant, ce point B est fixé à la main dans le code 
*/

int straightLine(float aLatitude, float aLongitude, float aVector[2], float& requestedThrottle, bool& reverse, float& requestedAngle, rclcpp::Logger logger){

    float bVector[2];
    float bVectorOrtho[2];

    float bLatitude = 43.570596;
    float bLongitude = 1.466500;

    //Cette section permet de réduire la vitesse proche du point B
    float distance = sqrt(pow(EARTH_RADIUS*(M_PI/180)*(bLatitude - aLatitude), 2) + pow(EARTH_RADIUS*(M_PI/180)*(bLongitude - aLongitude), 2));

    if (distance > MIN_DISTANCE_FOR_MAX_THROTTLE){
        requestedThrottle = 1.0;
    }else{
        requestedThrottle = 1.0 * distance / MIN_DISTANCE_FOR_MAX_THROTTLE;
    }

    if(requestedThrottle < MIN_THROTTLE){
        requestedThrottle = 0;
        reverse = false;
        requestedAngle = 0;
    }else{

        //Cette section permet de déterminer si la voiture doit aller à droite ou gauche

        bVector[0] = bLatitude - aLatitude ;
        bVector[1] = bLongitude - aLongitude ;
        bVectorOrtho[0] = -bVector[1];
        bVectorOrtho[1] = bVector[0];

        float angle = (180/M_PI) * (acos((aVector[0]*bVector[0] + aVector[1]*bVector[1])/(sqrt(pow(aVector[0], 2) + pow(aVector[1], 2)) * sqrt(pow(bVector[0], 2) + pow(bVector[1], 2)))));
        
        if (angle > MIN_ANGLE_FOR_MAX_STEERING){
            if (aVector[0]*bVectorOrtho[0] + aVector[1]*bVectorOrtho[1] > 0){
                requestedAngle = -1.0;
            }else{
                requestedAngle = 1.0;
            }
        }else{
            if (aVector[0]*bVectorOrtho[0] + aVector[1]*bVectorOrtho[1] > 0){
                requestedAngle = -1.0 * (angle/MIN_ANGLE_FOR_MAX_STEERING);
            }else{
                requestedAngle = 1.0 * (angle/MIN_ANGLE_FOR_MAX_STEERING);
            }
        }

        RCLCPP_INFO(logger, "Valeur de l'angle entre les vecteurs : %f", angle);

        reverse = false;

    }

    return 0;

}


/**
     * Represents a graph data structure.
     * 
     * A graph is a collection of nodes (vertices) and edges that connect these nodes.
     * It can be used to represent relationships between objects or entities.
**/
class Graph {

    unordered_map<char, const unordered_map<char, int>> vertices;
    
public:

    void add_vertex(char name, const unordered_map<char, int>& edges)
    {
        vertices.insert(unordered_map<char, const unordered_map<char, int>>::value_type(name, edges));
    }
    
    vector<char> shortest_path(char start, char finish)
    {
        unordered_map<char, int> distances;
        unordered_map<char, char> previous;
        vector<char> nodes;
        vector<char> path;
        
        auto comparator = [&] (char left, char right) { return distances[left] > distances[right]; };

        for (auto& vertex : vertices)
        {
            if (vertex.first == start)
            {
                distances[vertex.first] = 0;
            }
            else
            {
                distances[vertex.first] = numeric_limits<int>::max();
            }
            
            nodes.push_back(vertex.first);
            push_heap(begin(nodes), end(nodes), comparator);
        }
        
        while (!nodes.empty())
        {
            pop_heap(begin(nodes), end(nodes), comparator);
            char smallest = nodes.back();
            nodes.pop_back();
            
            if (smallest == finish)
            {
                while (previous.find(smallest) != end(previous))
                {
                    path.push_back(smallest);
                    smallest = previous[smallest];
                }
                
                break;
            }
            
            if (distances[smallest] == numeric_limits<int>::max())
            {
                break;
            }
            
            for (auto& neighbor : vertices[smallest])
            {
                int alt = distances[smallest] + neighbor.second;
                if (alt < distances[neighbor.first])
                {
                    distances[neighbor.first] = alt;
                    previous[neighbor.first] = smallest;
                    make_heap(begin(nodes), end(nodes), comparator);
                }
            }
        }
        
        return path;
    }

    void createGraph(map<char, float[2]> coordinates)
    {
            
        add_vertex('A', {{'B', distance(coordinates['A'], coordinates['B'])}, {'C', distance(coordinates['A'], coordinates['C'])}, {'D', distance(coordinates['A'], coordinates['D'])}});
        add_vertex('B', {{'A', distance(coordinates['B'], coordinates['A'])}, {'C', distance(coordinates['B'], coordinates['C'])}, {'D', distance(coordinates['B'], coordinates['D'])}});
        add_vertex('C', {{'A', distance(coordinates['C'], coordinates['A'])}, {'B', distance(coordinates['C'], coordinates['B'])}, {'D', distance(coordinates['C'], coordinates['D'])}});
        add_vertex('D', {{'A', distance(coordinates['D'], coordinates['A'])}, {'B', distance(coordinates['D'], coordinates['B'])}, {'C', distance(coordinates['D'], coordinates['C'])}});
        
    }

    void distance(float a[2], float b[2])
    {
        return sqrt(pow(EARTH_RADIUS*(M_PI/180)*(b[0] - a[0]), 2) + pow(EARTH_RADIUS*(M_PI/180)*(b[1] - a[1]), 2));
    }


};