#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float64.hpp"
#include <chrono>
#include <memory>
#include <functional>


// class MyCppNode : public rclcpp::Node
// {   
//     public:
//     MyCppNode() : Node("my_cpp_node")
//     {
//         RCLCPP_INFO(this->get_logger(), "Hello, ROS 2 C++ Node!");
//     }
// };

class MyNode : public rclcpp::Node
{
    public:
    MyNode();
 

    private:
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_F;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_I;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_S;
    rclcpp::TimerBase::SharedPtr timer_;

    
    int count_=0;

    void timer_callback();
    

};