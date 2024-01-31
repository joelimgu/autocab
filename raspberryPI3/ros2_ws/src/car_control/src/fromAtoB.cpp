#include "../include/car_control/fromAtoB.h"
#include <math.h>
#include <stdio.h>
#include <unordered_map>



using namespace std ;


/*
Cette fonction nous sert de test pour déplacer la voiture d'un point A (le point actuel) à un point B (le point de destination). Elle prend en cmpte le fait de faire un demi-tour.
*/

//bool straightLine(float aLatitude, float aLongitude, float bLatitude, float bLongitude, float aVector[2], float& requestedThrottle, bool& reverse, float& requestedAngle, rclcpp::Logger logger){
bool straightLine(float aLatitude, float aLongitude, float bLatitude, float bLongitude, float aVector[2], float& requestedThrottle, bool& reverse, float& requestedAngle, rclcpp::Logger logger,float& traj_past_angle_error,float& traj_current_angle_error){


    bool arrived;
    float bVector[2];
    float bVectorOrtho[2];

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
        arrived = true;

    }else{

        //Cette section permet de déterminer si la voiture doit aller à droite ou gauche, avancer ou reculer

        bVector[0] = bLatitude - aLatitude ;
        bVector[1] = bLongitude - aLongitude ;
        bVectorOrtho[0] = -bVector[1];
        bVectorOrtho[1] = bVector[0];

        float angle = (180/M_PI) * (acos((aVector[0]*bVector[0] + aVector[1]*bVector[1])/(sqrt(pow(aVector[0], 2) + pow(aVector[1], 2)) * sqrt(pow(bVector[0], 2) + pow(bVector[1], 2)))));
        
        bool turn_right = aVector[0]*bVectorOrtho[0] + aVector[1]*bVectorOrtho[1] > 0;
        bool wrong_orientation = aVector[0]*bVector[0] + aVector[1]*bVector[1] < 0;
        if (wrong_orientation){
            reverse = true;
            if (turn_right){
                requestedAngle = 1.0;
            }else{
                requestedAngle = -1.0;
            }
        // } else {
        //     reverse = false;
        //     if (angle > MIN_ANGLE_FOR_MAX_STEERING){
        //         if (turn_right){
        //             requestedAngle = -1.0;
        //         }else{
        //             requestedAngle = 1.0;
        //         }
        //     }else{
        //         if (turn_right){
        //             requestedAngle = -1.0 * (angle/40);
        //         }else{
        //             requestedAngle = 1.0 * (angle/40);
        //         }
        //     }
        // }
        } else {
            reverse = false;
            float oriented_angle;
            if (turn_right)
            {
                oriented_angle = angle;
            }
            else
            {
                oriented_angle = -angle;
            }
            correctTrajAngle(requestedAngle,traj_past_angle_error,traj_current_angle_error,oriented_angle);
        }
        
        RCLCPP_INFO(logger, "Valeur de l'angle entre les vecteurs : %f et de reverse : %d", angle, reverse);
        RCLCPP_INFO(logger, "Valeur de traj_past_error : %f traj current error : %f, requested %f",traj_past_angle_error ,traj_current_angle_error,requestedAngle);

        arrived = false;

    }

    return arrived;

}


void Graph::add_vertex(char name, const std::unordered_map<char, float>& edges)
{
    vertices.insert(std::unordered_map<char, const std::unordered_map<char, float>>::value_type(name, edges));
}

vector<char> Graph::shortest_path(char start, char finish)
{
    std::unordered_map<char, float> distances;
    std::unordered_map<char, char> previous;
    std::vector<char> nodes;
    std::vector<char> path;
    
    auto comparator = [&] (char left, char right) { return distances[left] > distances[right]; };

    for (auto& vertex : vertices)
    {
        if (vertex.first == start)
        {
            distances[vertex.first] = 0;
        }
        else
        {
            distances[vertex.first] = numeric_limits<float>::max();
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
            float alt = distances[smallest] + neighbor.second;
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

void Graph::createGraph(std::map<char, float[2]> coordinates)
{
    /* Changer cette partie , on ne peut pas créer des maps avec {}*/
    add_vertex('A', {{'B', distance(coordinates['A'], coordinates['B'])}, {'D', distance(coordinates['A'], coordinates['D'])}});
    add_vertex('B', {{'A', distance(coordinates['B'], coordinates['A'])}, {'C', distance(coordinates['B'], coordinates['C'])}, {'F', distance(coordinates['B'], coordinates['F'])}});
    add_vertex('C', {{'B', distance(coordinates['C'], coordinates['B'])}, {'K', distance(coordinates['C'], coordinates['K'])}});
    add_vertex('D', {{'A', distance(coordinates['D'], coordinates['A'])}, {'E', distance(coordinates['D'], coordinates['E'])}});
    add_vertex('E', {{'D', distance(coordinates['E'], coordinates['D'])}, {'G', distance(coordinates['E'], coordinates['G'])}});
    add_vertex('F', {{'B', distance(coordinates['F'], coordinates['B'])}});
    add_vertex('G', {{'E', distance(coordinates['G'], coordinates['E'])}, {'H', distance(coordinates['G'], coordinates['H'])}});
    add_vertex('H', {{'G', distance(coordinates['H'], coordinates['G'])}, {'I', distance(coordinates['H'], coordinates['I'])}});
    add_vertex('I', {{'H', distance(coordinates['I'], coordinates['H'])}, {'J', distance(coordinates['I'], coordinates['J'])}});
    add_vertex('J', {{'I', distance(coordinates['J'], coordinates['I'])}, {'K', distance(coordinates['J'], coordinates['K'])}});
    add_vertex('K', {{'J', distance(coordinates['K'], coordinates['J'])}, {'C', distance(coordinates['K'], coordinates['C'])}});
}

float distance(float a[2], float b[2])
{
    return sqrt(pow(EARTH_RADIUS*(M_PI/180)*(b[0] - a[0]), 2) + pow(EARTH_RADIUS*(M_PI/180)*(b[1] - a[1]), 2));
}

char detectClosestPoint(float currentLat, float currentLon,std::map<char, float[2]> &pointMap)
{
    /* Calculer les distances entre les coordonnees de la voiture et chaque point */
    float minDistance = -1;
    char point = '0';
    float currentPos[2];
    currentPos[0] = currentLat;
    currentPos[1] = currentLon;
    /* Use iterator to iterate on the coordinates of the pointMap and find the min distance */
    for (std::map<char, float[2]>::iterator it = pointMap.begin(); it != pointMap.end(); ++it)
    {
        float computedDistance = distance(currentPos,it->second);
        if((minDistance == -1) || computedDistance<minDistance)
        {
            minDistance = computedDistance;
            point = it->first;
        }
    }
    return point;
}


