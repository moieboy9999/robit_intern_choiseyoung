#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float64.hpp"
#include <chrono>
#include <memory>
#include <functional>

class MyNode : public rclcpp::Node
{
    public:
    MyNode();

    private:
    
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subS;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subI;
    rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subF;

    void topic_callbackS(const std_msgs::msg::String::SharedPtr msgS);
    void topic_callbackI(const std_msgs::msg::Int32::SharedPtr msgI);
    void topic_callbackF(const std_msgs::msg::Float64::SharedPtr msgF);

};