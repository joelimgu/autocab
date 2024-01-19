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
#include "interfaces/msg/toserveur.hpp"
#include "interfaces/msg/ultrasonic.hpp"

#include "std_srvs/srv/empty.hpp"

#include "../include/car_control/steeringCmd.h"
#include "../include/car_control/propulsionCmd.h"
#include "../include/car_control/corrector.h"
#include "../include/car_control/car_control_node.h"
#include "../include/car_control/fromAtoB.h"
#include "../include/car_control/obstacle_detection.h"
#include "../include/car_control/odometry.h"


#define PI 3.1415926535897932384626433832795    
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

        /* Initialising the car's state for odometry, should create my own callback to initialise it */
        odom::initialise_position(past_reverse_odom,past_position_odom,current_position_odom,past_speeds_odom,past_theta_odom,current_theta_odom);

        //Vrai initialisation
        departurePoint = 'A';
        finalPoint = 'A';
        currentPoint = 'A' ;
        departurePointReached = true;
        finalPointReached = true;
        arrivedAtCurrentPoint = true;
        requestNumber = 0;

        // //initialisation pour les tests
        // departurePoint = 'A';
        // finalPoint = 'C';
        // departurePointReached = false;
        // finalPointReached = false;
        // arrivedAtCurrentPoint = false;
        // requestNumber = 0;
        
        (coordinates['A'])[0] = 43.570593;
        (coordinates['A'])[1] = 1.466513;
        (coordinates['B'])[0] = 43.570853;
        (coordinates['B'])[1] = 1.467021;
        (coordinates['C'])[0] = 43.571061;
        (coordinates['C'])[1] = 1.466837;
        (coordinates['D'])[0] = 43.570342;
        (coordinates['D'])[1] = 1.466029;
        (coordinates['E'])[0] = 43.570567;
        (coordinates['E'])[1] = 1.465800;
        (coordinates['F'])[0] = 43.570671;
        (coordinates['F'])[1] = 1.467218;
        (coordinates['G'])[0] = 43.570706;
        (coordinates['G'])[1] = 1.465661;
        (coordinates['H'])[0] = 43.570942;
        (coordinates['H'])[1] = 1.465445;
        (coordinates['I'])[0] = 43.571185;
        (coordinates['I'])[1] = 1.465951;
        (coordinates['J'])[0] = 43.571440;
        (coordinates['J'])[1] = 1.466431;
        (coordinates['K'])[0] = 43.571233;
        (coordinates['K'])[1] = 1.466667;
        

        graph.createGraph(coordinates);

        // //tests pour prouver que le calcul de plus court chemin fonctionne
        // pathToDeparturePoint = graph.shortest_path(currentPoint, departurePoint);
        // if (pathToDeparturePoint.empty()){
        //     departurePointReached = true;
        // }
        // pathToFinalPoint = graph.shortest_path(departurePoint, finalPoint);
        // RCLCPP_INFO(this->get_logger(), "pathtofinalpoint : %c, %c", pathToFinalPoint[0], pathToFinalPoint[1]);
        // if (pathToFinalPoint.empty()){
        //     finalPointReached = true;
        // }


        publisher_can_= this->create_publisher<interfaces::msg::MotorsOrder>("motors_order", 10);

        publisher_steeringCalibration_ = this->create_publisher<interfaces::msg::SteeringCalibration>("steering_calibration", 10);

        publisher_to_serveur_= this->create_publisher<interfaces::msg::Toserveur>("to_serveur", 10);


        

        subscription_joystick_order_ = this->create_subscription<interfaces::msg::JoystickOrder>(
        "joystick_order", 10, std::bind(&car_control::joystickOrderCallback, this, _1));

        subscription_motors_feedback_ = this->create_subscription<interfaces::msg::MotorsFeedback>(
        "motors_feedback", 10, std::bind(&car_control::motorsFeedbackCallback, this, _1));

        subscription_steering_calibration_ = this->create_subscription<interfaces::msg::SteeringCalibration>(
        "steering_calibration", 10, std::bind(&car_control::steeringCalibrationCallback, this, _1));

        subscription_gnss_data_ = this->create_subscription<interfaces::msg::Gnss>(
        "gnss_data", 10, std::bind(&car_control::gnssDataCallback, this, _1));
        /*
        subscription_serveur_data_ = this->create_subscription<interfaces::msg::Serveur>(
        "serveur_data", 10, std::bind(&car_control::serveurDataCallback, this, _1));
        */
        subscription_us_data = this->create_subscription<interfaces::msg::Ultrasonic>(
        "us_data", 10, std::bind(&car_control::usDataCallback, this, _1));


        
        server_calibration_ = this->create_service<std_srvs::srv::Empty>(
                            "steering_calibration", std::bind(&car_control::steeringCalibration, this, std::placeholders::_1, std::placeholders::_2));

        timer_ = this->create_wall_timer(PERIOD_UPDATE_CMD, std::bind(&car_control::updateCmd, this));

        
        RCLCPP_INFO(this->get_logger(), "car_control_node READY");


    }

    /* Update data from us_data [callback function]  :
    *
    * This function is called when a message is published on the "/us_data" topic
    * 
    */
    void usDataCallback(const interfaces::msg::Ultrasonic & usData){
        front_left = usData.front_left;
        front_center = usData.front_center;
        front_right = usData.front_right;

        rear_left = usData.rear_left;
        rear_center = usData.rear_center;
        rear_right = usData.rear_right;
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
        past_steeringAngle_odom = currentAngle;
        currentAngle = motorsFeedback.steering_angle;
        leftRearRPM = motorsFeedback.left_rear_speed;
        rightRearRPM = motorsFeedback.right_rear_speed;
        leaftNticks= motorsFeedback.left_rear_odometry;
        rightNticks= motorsFeedback.right_rear_odometry;

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
        auto toServeur = interfaces::msg::Toserveur();

        toServeur.current_latitude = currentLatitude;
        toServeur.current_longitude = currentLongitude;

        if (!start)
        {    //Car stopped
            leftRearPwmCmd = STOP;
            rightRearPwmCmd = STOP;
            steeringPwmCmd = STOP;

            toServeur.on = false;
            toServeur.arrived = false;
            toServeur.mode = 0;
        }
        else
        { //Car started

            toServeur.on = true;

            //Manual Mode
            if (mode==0)
            {

                toServeur.mode = 0;
                toServeur.arrived = false;

            //Autonomous Mode
            } else if (mode==1){

                toServeur.mode = 1;

                if (!arrivedAtCurrentPoint){

                    arrivedAtCurrentPoint = straightLine(currentLatitude, currentLongitude, (coordinates[currentPoint])[0], (coordinates[currentPoint])[1], currentDirection, requestedThrottle, reverse, requestedSteerAngle, this->get_logger());
                    RCLCPP_INFO(this->get_logger(), "En déplaçement vers le currentpoint %c", currentPoint);

                } else {

                    //Ici il faut toujours pouvoir garder le controle de la voiture avec la manette. 
                    //Dans chaque cas, parcours le path correspondant. Une fois arrivé à destination, enlever le premier point de la liste
                    bool arrived = false ;
                    if (!departurePointReached){
                        RCLCPP_INFO(this->get_logger(), "En déplaçement vers le departurepoint %c", pathToDeparturePoint[pathToDeparturePoint.size()-1]);
                        arrived = straightLine(currentLatitude, currentLongitude, (coordinates[pathToDeparturePoint[pathToDeparturePoint.size()-1]])[0], (coordinates[pathToDeparturePoint[pathToDeparturePoint.size()-1]])[1], currentDirection, requestedThrottle, reverse, requestedSteerAngle, this->get_logger());
                        if (arrived == true){
                            pathToDeparturePoint.erase(pathToDeparturePoint.end()-1) ;
                            if (pathToDeparturePoint.empty()){
                                departurePointReached = true;
                                sleep(5); //On attend 5 secondes avant de partir
                            }
                        }
                    } else if (!finalPointReached){
                        RCLCPP_INFO(this->get_logger(), "En déplaçement vers le finalpoint %c", pathToFinalPoint[pathToFinalPoint.size()-1]);
                        arrived = straightLine(currentLatitude, currentLongitude, (coordinates[pathToFinalPoint[pathToFinalPoint.size()-1]])[0], (coordinates[pathToFinalPoint[pathToFinalPoint.size()-1]])[1], currentDirection, requestedThrottle, reverse, requestedSteerAngle, this->get_logger());
                        if (arrived == true){
                            pathToFinalPoint.erase(pathToFinalPoint.end()-1) ;
                            if (pathToFinalPoint.empty()){
                                finalPointReached = true;
                                sleep(5); //On attend 5 secondes avant de partir
                            }
                        }
                    } else {
                        requestedThrottle = 0;
                        requestedSteerAngle = 0;
                        reverse = false ;
                    }

                    toServeur.arrived = (arrivedAtCurrentPoint && departurePointReached && finalPointReached);

                }
            }

    void UpdateOdometrie(){
        Rdistance= phi * D * (rightNticks/(double)36);
        Ldistance= phi * D * (leaftNticks/(double)36);
        Center= (Rdistance + Ldistance)/2;
        Xpos = Xpos + Center*cos(phi);
        Ypos = Ypos + Center*sin(phi);
        phi = phi + (Rdistance-Ldistance)/longitud;
        phi = atan2(sin(phi),cos(phi));
 
    }

            /* Left wheel error and PWM */
            correctWheelSpeed(leftRearPwmCmd,left_past_pwm_error,left_current_pwm_error,leftRearRPM,0);
            /* Right wheel error and PWM */
            correctWheelSpeed(rightRearPwmCmd,right_past_pwm_error,right_current_pwm_error,rightRearRPM,0);

            manualPropulsionCmd(requestedThrottle, reverse, leftRearPwmCmd,rightRearPwmCmd);
            steeringCmd(requestedSteerAngle,currentAngle, steeringPwmCmd);

            /* Calculating future position with current values using odometry , using wheel radius R and distance between front and rear of the car */

            odom::estimate_pos(0.01,0.55,past_reverse_odom,reverse,0.1,past_steeringAngle_odom,past_theta_odom,current_theta_odom,past_speeds_odom,
                    leftRearRPM,
                    rightRearRPM,
                    past_position_odom, 
                    current_position_odom);
            RCLCPP_INFO(this->get_logger(), "Odometry positions : X = %f Y = %f \n Current theta %f \n", current_position_odom[0],current_position_odom[1],current_theta_odom);
            

            UpdateOdometrie();
            RCLCPP_INFO(this->get_logger(), "Odometry positions 2 : X = %f Y = %f \n Current theta %f \n", Xpos ,Ypos, phi);



            /*
            //Obstacle Detection in all modes
            if (ObstacleCmdFront(front_left, front_center, front_right) == STOP && reverse == false){
                leftRearPwmCmd = STOP;
                rightRearPwmCmd = leftRearPwmCmd;
            }

            if (ObstacleCmdRear(rear_left, rear_center, rear_right) == STOP && reverse == true){
                leftRearPwmCmd = STOP;
                rightRearPwmCmd = leftRearPwmCmd;
            }
            */
        }

        //Send order to serveur
        publisher_to_serveur_->publish(toServeur);

        
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
    void steeringCalibrationCallback (const interfaces::msg::SteeringCalibration & calibrationMsg)
    {
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
            if (reverse==true){
                currentDirection[0]= -currentDirection[0];
                currentDirection[1]= -currentDirection[1];
            }
            currentLatitude = gnssData.latitude;
            currentLongitude = gnssData.longitude;
            currentPoint = detectClosestPoint(currentLatitude, currentLongitude, coordinates);
            RCLCPP_INFO(this->get_logger(), "Data GPS SHutUppdated, currentDirection = [%f, %f]", currentDirection[0], currentDirection[1]);
            RCLCPP_INFO(this->get_logger(), "currentPoint = %c", currentPoint);
        }
    }

    /* Update departurePoint and finalPoint from serveur_data [callback function]  :
    *
    * This function is called when a message is published on the "/serveur_data" topic
    * 
    */
    /*
    void serveurDataCallback(const interfaces::msg::Serveur & serveurData)
    {
        is_request_fullfilled = mode==1 && arrivedAtCurrentPoint && departurePointReached && finalPointReached 
        if (is_request_fullfilled && (requestNumber != serveurData.request_number)){
            departurePointReached = false;
            finalPointReached = false;
            arrivedAtCurrentPoint = false;
            departurePoint = serveurData.departure_point;
            finalPoint = serveurData.final_point;
            requestNumber = serveurData.request_number;
            pathToDeparturePoint = graph.shortest_path(currentPoint, departurePoint);
            if (pathToDeparturePoint.empty()){
                departurePointReached = true;
            }
            pathToFinalPoint = graph.shortest_path(departurePoint, finalPoint);
            if (pathToFinalPoint.empty()){
                finalPointReached = true;
            }
            RCLCPP_INFO(this->get_logger(), "Data serveur updated, departurePoint = %c, finalPoint = %c", departurePoint, finalPoint);
        }
        
    }
    */
    


    // ---- Private variables ----

    //General variables
    bool start;
    int mode;    //0 : Manual    1 : Auto    2 : Calibration

    
    //Motors feedback variables
    float currentAngle;



    /* Automatic control mode variables */
    float right_past_pwm_error = 0;
    float right_current_pwm_error = 0;
    float left_past_pwm_error = 0;
    float left_current_pwm_error = 0;

    /* Odometry variables */
    float past_reverse_odom;
    float past_steeringAngle_odom;
    float past_position_odom[2];
    float current_position_odom[2];
    float past_theta_odom;
    float current_theta_odom;
    float past_speeds_odom[2] /* Vector containing the past speeds for both right and left rear wheels */
    /* Odometry variables 2*/
    float phi=0:
    float D=0.03;
    float leaftNticks;
    float rightNticks;
    float Xpos=0;
    float Ypos=0;

    //Manual Mode variables (with joystick control)
    bool reverse;
    float requestedThrottle;
    float requestedSteerAngle;

    //Control variables
    float leftRearPwmCmd;
    float rightRearPwmCmd;
    float smallRightRearPwmCmd = 0.0;
    float smallLeftRearPwmCmd = 0.0;
    float leftRearRPM;
    float rightRearRPM;
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
    
    //us data variables
    int16_t front_left;
    int16_t front_center;
    int16_t front_right;

    int16_t rear_left;
    int16_t rear_center;
    int16_t rear_right;

    //Publishers
    rclcpp::Publisher<interfaces::msg::MotorsOrder>::SharedPtr publisher_can_;
    rclcpp::Publisher<interfaces::msg::SteeringCalibration>::SharedPtr publisher_steeringCalibration_;
    rclcpp::Publisher<interfaces::msg::Toserveur>::SharedPtr publisher_to_serveur_;

    //Subscribers
    rclcpp::Subscription<interfaces::msg::JoystickOrder>::SharedPtr subscription_joystick_order_;
    rclcpp::Subscription<interfaces::msg::MotorsFeedback>::SharedPtr subscription_motors_feedback_;
    rclcpp::Subscription<interfaces::msg::SteeringCalibration>::SharedPtr subscription_steering_calibration_;
    rclcpp::Subscription<interfaces::msg::Gnss>::SharedPtr subscription_gnss_data_;
    rclcpp::Subscription<interfaces::msg::Serveur>::SharedPtr subscription_serveur_data_;
    rclcpp::Subscription<interfaces::msg::Ultrasonic>::SharedPtr subscription_us_data;

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
