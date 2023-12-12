#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <math.h>
#include <unordered_map>

#include "interfaces/msg/motors_order.hpp"
#include "interfaces/msg/motors_feedback.hpp"
#include "interfaces/msg/steering_calibration.hpp"
#include "interfaces/msg/joystick_order.hpp"
#include "interfaces/msg/gnss.hpp"
#include "interfaces/msg/serveur.hpp"

#include "std_srvs/srv/empty.hpp"

#include "../include/car_control/steeringCmd.h"
#include "../include/car_control/propulsionCmd.h"
#include "../include/car_control/car_control_node.h"
#include "../include/car_control/fromAtoB.h"

using namespace std;
using placeholders::_1;


class car_control : public rclcpp::Node {

public:
    car_control()
    : Node("car_control_node")
    {
        start = false;
        mode = 0;

        requestedThrottle = 0;
        requestedSteerAngle = 0;
        currentLatitude = 0;
        currentLongitude = 0;
        currentDirection[0]=1;
        currentDirection[1]=1;

        departurePoint = 'A';
        finalPoint = 'A';
        currentPoint = 'A' ;
        departurePointReached = true;
        finalPointReached = true;
        arrivedAtCurrentPoint = true;
        requestNumber = 0;
        
        (coordinates['A'])[0] = 43.570596;
        (coordinates['A'])[1] = 1.466500;
        (coordinates['B'])[0] = 43.570596;
        (coordinates['B'])[1] = 1.466501;
        (coordinates['C'])[0] = 43.570596;
        (coordinates['C'])[1] = 1.466502;
        (coordinates['D'])[0] = 43.570596;
        (coordinates['D'])[1] = 1.466503;
        

        graph.createGraph(coordinates);
    

        publisher_can_= this->create_publisher<interfaces::msg::MotorsOrder>("motors_order", 10);

        publisher_steeringCalibration_ = this->create_publisher<interfaces::msg::SteeringCalibration>("steering_calibration", 10);

        

        subscription_joystick_order_ = this->create_subscription<interfaces::msg::JoystickOrder>(
        "joystick_order", 10, std::bind(&car_control::joystickOrderCallback, this, _1));

        subscription_motors_feedback_ = this->create_subscription<interfaces::msg::MotorsFeedback>(
        "motors_feedback", 10, std::bind(&car_control::motorsFeedbackCallback, this, _1));

        subscription_steering_calibration_ = this->create_subscription<interfaces::msg::SteeringCalibration>(
        "steering_calibration", 10, std::bind(&car_control::steeringCalibrationCallback, this, _1));

        subscription_gnss_data_ = this->create_subscription<interfaces::msg::Gnss>(
        "gnss_data", 10, std::bind(&car_control::gnssDataCallback, this, _1));

        subscription_serveur_data_ = this->create_subscription<interfaces::msg::Serveur>(
        "serveur_data", 10, std::bind(&car_control::serveurDataCallback, this, _1));


        
        server_calibration_ = this->create_service<std_srvs::srv::Empty>(
                            "steering_calibration", std::bind(&car_control::steeringCalibration, this, std::placeholders::_1, std::placeholders::_2));

        timer_ = this->create_wall_timer(PERIOD_UPDATE_CMD, std::bind(&car_control::updateCmd, this));

        
        RCLCPP_INFO(this->get_logger(), "car_control_node READY");
    }

    
private:

    /* Update start, mode, requestedThrottle, requestedSteerAngle and reverse from joystick order [callback function]  :
    *
    * This function is called when a message is published on the "/joystick_order" topic
    * 
    */
    void joystickOrderCallback(const interfaces::msg::JoystickOrder & joyOrder) {

        if (joyOrder.start != start){
            start = joyOrder.start;

            if (start)
                RCLCPP_INFO(this->get_logger(), "START");
            else 
                RCLCPP_INFO(this->get_logger(), "STOP");
        }
        

        if (joyOrder.mode != mode && joyOrder.mode != -1){ //if mode change
            mode = joyOrder.mode;

            if (mode==0){
                RCLCPP_INFO(this->get_logger(), "Switching to MANUAL Mode");
                requestNumber = 0 ;
                departurePointReached = true ;
                finalPointReached = true ;
                arrivedAtCurrentPoint = true ;
            }else if (mode==1){
                RCLCPP_INFO(this->get_logger(), "Switching to AUTONOMOUS Mode");
            }else if (mode==2){
                RCLCPP_INFO(this->get_logger(), "Switching to STEERING CALIBRATION Mode");
                startSteeringCalibration();
            }
        }
        
        if (mode == 0 && start){  //if manual mode -> update requestedThrottle, requestedSteerAngle and reverse from joystick order
            requestedThrottle = joyOrder.throttle;
            requestedSteerAngle = joyOrder.steer;
            reverse = joyOrder.reverse;
        }
    }

    /* Update currentAngle from motors feedback [callback function]  :
    *
    * This function is called when a message is published on the "/motors_feedback" topic
    * 
    */
    void motorsFeedbackCallback(const interfaces::msg::MotorsFeedback & motorsFeedback){
        currentAngle = motorsFeedback.steering_angle;
    }


    /* Update PWM commands : leftRearPwmCmd, rightRearPwmCmd, steeringPwmCmd
    *
    * This function is called periodically by the timer [see PERIOD_UPDATE_CMD in "car_control_node.h"]
    * 
    * In MANUAL mode, the commands depends on :
    * - requestedThrottle, reverse, requestedSteerAngle [from joystick orders]
    * - currentAngle [from motors feedback]
    */
    void updateCmd(){

        auto motorsOrder = interfaces::msg::MotorsOrder();

        if (!start){    //Car stopped
            leftRearPwmCmd = STOP;
            rightRearPwmCmd = STOP;
            steeringPwmCmd = STOP;


        }else{ //Car started

            //Manual Mode
            if (mode==0){
                
                manualPropulsionCmd(requestedThrottle, reverse, leftRearPwmCmd,rightRearPwmCmd);

                steeringCmd(requestedSteerAngle,currentAngle, steeringPwmCmd);


            //Autonomous Mode
            } else if (mode==1){
                

                if (!arrivedAtCurrentPoint){

                    arrivedAtCurrentPoint = straightLine(currentLatitude, currentLongitude, (coordinates[currentPoint])[0], (coordinates[currentPoint])[1], currentDirection, requestedThrottle, reverse, requestedSteerAngle, this->get_logger())

                } else {

                     //Ici on met a jour les variables departurePointReached et finalPointReached
                    if (!departurePointReached && pathToDeparturePoint.empty()){
                        departurePointReached = true;
                        waitingTime(5); //On attend 5 secondes avant de partir
                    } else if (!finalPointReached && pathToFinalPoint.empty()){
                        finalPointReached = true;
                        waitingTime(5); //On attend 5 secondes avant de partir
                    }
                    //Ici il faut toujours pouvoir garder le controle de la voiture avec la manette. 
                    //Dans chaque cas, parcours le path correspondant. Une fois arrivé à destination, enlever le premier point de la liste
                    bool arrived = false ;
                    if (!departurePointReached){
                        arrived = straightLine(currentLatitude, currentLongitude, (coordinates[pathToDeparturePoint[0]])[0], (coordinates[pathToDeparturePoint[0]])[1], currentDirection, requestedThrottle, reverse, requestedSteerAngle, this->get_logger());
                        if (arrived == true){
                            pathToDeparturePoint.erase(pathToDeparturePoint.begin()) ;
                        }
                    } else if (!finalPointReached){
                        straightLine(currentLatitude, currentLongitude, (coordinates[pathToFinalPoint[0]])[0], (coordinates[pathToFinalPoint[0]])[1], currentDirection, requestedThrottle, reverse, requestedSteerAngle, this->get_logger());
                        if (arrived == true){
                            pathToFinalPoint.erase(pathToFinalPoint.begin()) ;
                        }
                    } else {
                        requestedThrottle = 0;
                        requestedSteerAngle = 0;
                        reverse = false ;
                    }

                }

                manualPropulsionCmd(requestedThrottle, reverse, leftRearPwmCmd,rightRearPwmCmd);
                steeringCmd(requestedSteerAngle,currentAngle, steeringPwmCmd);

            }
        }


        //Send order to motors
        motorsOrder.left_rear_pwm = leftRearPwmCmd;
        motorsOrder.right_rear_pwm = rightRearPwmCmd;
        motorsOrder.steering_pwm = steeringPwmCmd;


        publisher_can_->publish(motorsOrder);
    }


    /* Start the steering calibration process :
    *
    * Publish a calibration request on the "/steering_calibration" topic
    */
    void startSteeringCalibration(){

        auto calibrationMsg = interfaces::msg::SteeringCalibration();
        calibrationMsg.request = true;

        RCLCPP_INFO(this->get_logger(), "Sending calibration request .....");
        publisher_steeringCalibration_->publish(calibrationMsg);
    }


    /* Function called by "steering_calibration" service
    * 1. Switch to calibration mode
    * 2. Call startSteeringCalibration function
    */
    void steeringCalibration([[maybe_unused]] std_srvs::srv::Empty::Request::SharedPtr req,
                            [[maybe_unused]] std_srvs::srv::Empty::Response::SharedPtr res)
    {

        mode = 2;    //Switch to calibration mode
        RCLCPP_WARN(this->get_logger(), "Switching to STEERING CALIBRATION Mode");
        startSteeringCalibration();
    }
    

    /* Manage steering calibration process [callback function]  :
    *
    * This function is called when a message is published on the "/steering_calibration" topic
    */
    void steeringCalibrationCallback (const interfaces::msg::SteeringCalibration & calibrationMsg){

        if (calibrationMsg.in_progress == true && calibrationMsg.user_need == false){
        RCLCPP_INFO(this->get_logger(), "Steering Calibration in progress, please wait ....");

        } else if (calibrationMsg.in_progress == true && calibrationMsg.user_need == true){
            RCLCPP_WARN(this->get_logger(), "Please use the buttons (L/R) to center the steering wheels.\nThen, press the blue button on the NucleoF103 to continue");
        
        } else if (calibrationMsg.status == 1){
            RCLCPP_INFO(this->get_logger(), "Steering calibration [SUCCESS]");
            RCLCPP_INFO(this->get_logger(), "Switching to MANUAL Mode");
            mode = 0;    //Switch to manual mode
            start = false;  //Stop car
        
        } else if (calibrationMsg.status == -1){
            RCLCPP_ERROR(this->get_logger(), "Steering calibration [FAILED]");
            RCLCPP_INFO(this->get_logger(), "Switching to MANUAL Mode");
            mode = 0;    //Switch to manual mode
            start = false;  //Stop car
        }
    }

    /* Update currentLatitude and currentLongitude from gnss_data [callback function]  :
    *
    * This function is called when a message is published on the "/gnss_data" topic
    * 
    */

    void gnssDataCallback(const interfaces::msg::Gnss & gnssData)
    {
        if ((abs(currentLatitude-gnssData.latitude) >= MIN_UPDATE_COORDINATES) || (abs(currentLongitude-gnssData.longitude) >= MIN_UPDATE_COORDINATES)){
            currentDirection[0] =  gnssData.latitude - currentLatitude ; 
            currentDirection[1] =  gnssData.longitude - currentLongitude;
            currentLatitude = gnssData.latitude;
            currentLongitude = gnssData.longitude;
            currentPoint = detectClosestPoint(currentLatitude, currentLongitude, coordinates);
            RCLCPP_INFO(this->get_logger(), "Data GPS updated, currentDirection = [%f, %f]", currentDirection[0], currentDirection[1]);
        }
        
        RCLCPP_INFO(this->get_logger(), "Data GPS stored, currentDirection = [%f, %f]", currentDirection[0], currentDirection[1]);
    }

    /* Update departurePoint and finalPoint from serveur_data [callback function]  :
    *
    * This function is called when a message is published on the "/serveur_data" topic
    * 
    */

    void serveurDataCallback(const interfaces::msg::Serveur & serveurData)
    {
        if (mode==1 && departurePointReached && finalPointReached && (requestNumber != serveurData.request_number)){
            departurePointReached = false;
            finalPointReached = false;
            arrivedAtCurrentPoint = false;
            departurePoint = serveurData.departure_point;
            finalPoint = serveurData.final_point;
            requestNumber = serveurData.request_number;
            pathToDeparturePoint = graph.shortest_path(currentPoint, departurePoint);
            pathToFinalPoint = graph.shortest_path(departurePoint, finalPoint);
            RCLCPP_INFO(this->get_logger(), "Data serveur updated, departurePoint = %c, finalPoint = %c", departurePoint, finalPoint);
        }
        
    }
    


    // ---- Private variables ----

    //General variables
    bool start;
    int mode;    //0 : Manual    1 : Auto    2 : Calibration

    
    //Motors feedback variables
    float currentAngle;

    //Manual Mode variables (with joystick control)
    bool reverse;
    float requestedThrottle;
    float requestedSteerAngle;

    //Control variables
    uint8_t leftRearPwmCmd;
    uint8_t rightRearPwmCmd;
    uint8_t steeringPwmCmd;

    //gnss data variables
    float currentLatitude;
    float currentLongitude;
    float currentDirection[2];

    //trajectory variables
    char departurePoint;
    char finalPoint;
    bool departurePointReached;
    bool finalPointReached;
    int requestNumber;
    Graph graph;
    map<char, float[2]> coordinates;
    vector<char> pathToDeparturePoint;
    vector<char> pathToFinalPoint;    
    char currentPoint;
    bool arrivedAtCurrentPoint;

    //Publishers
    rclcpp::Publisher<interfaces::msg::MotorsOrder>::SharedPtr publisher_can_;
    rclcpp::Publisher<interfaces::msg::SteeringCalibration>::SharedPtr publisher_steeringCalibration_;

    //Subscribers
    rclcpp::Subscription<interfaces::msg::JoystickOrder>::SharedPtr subscription_joystick_order_;
    rclcpp::Subscription<interfaces::msg::MotorsFeedback>::SharedPtr subscription_motors_feedback_;
    rclcpp::Subscription<interfaces::msg::SteeringCalibration>::SharedPtr subscription_steering_calibration_;
    rclcpp::Subscription<interfaces::msg::Gnss>::SharedPtr subscription_gnss_data_;
    rclcpp::Subscription<interfaces::msg::Serveur>::SharedPtr subscription_serveur_data_;

    //Timer
    rclcpp::TimerBase::SharedPtr timer_;

    //Steering calibration Service
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr server_calibration_;
};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<car_control>();

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}