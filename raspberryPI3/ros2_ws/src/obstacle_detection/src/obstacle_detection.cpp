#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/string.hpp"

#include "interfaces/msg/ultrasonic.hpp"


using namespace std;
using placeholders::_1;


class obstacle_detection : public rclcpp::Node {

public:
    obstacle_detection()
    : Node("obstacle_detection_node")
    {
        subscription_us_data = this->create_subscription<interfaces::msg::Ultrasonic>(
        "us_data", 10, std::bind(&obstacle_detection::detectionCallback, this, _1));
    }

    
private:
    void detectionCallback(const interfaces::msg::Ultrasonic & const std_msgs::msg::String::SharedPtr msg) {
        
        RCLCPP_INFO(get_logger(), "Received message: '%s'", msg->data.c_str());
    }

    rclcpp::Subscription<interfaces::msg::Ultrasonic>::SharedPtr subscription_us_data;
};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<obstacle_detection>();

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
