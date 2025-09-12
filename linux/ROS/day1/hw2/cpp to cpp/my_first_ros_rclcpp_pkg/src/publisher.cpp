#include "../include/my_first_ros_rclcpp_pkg/publisher.hpp"

MyNode::MyNode() : Node("p")
{
    publisher_S = this->create_publisher<std_msgs::msg::String>("tS", 10);
    publisher_I = this->create_publisher<std_msgs::msg::Int32>("tI", 10);
    publisher_F = this->create_publisher<std_msgs::msg::Float64>("tF", 10);
    timer_ = this->create_wall_timer(std::chrono::duration<double>(1.0), std::bind(&MyNode::timer_callback, this));

}

void MyNode::timer_callback() {

auto msgS =  std_msgs::msg::String();
msgS.data = "Hello World : " + std::to_string(count_++);
RCLCPP_INFO(this->get_logger(), "Published message S: '%s'",msgS.data.c_str());
publisher_S->publish(msgS);

auto msgI =  std_msgs::msg::Int32();
msgI.data = count_;
RCLCPP_INFO(this->get_logger(), "Published message I: '%d'",msgI.data);
publisher_I->publish(msgI);

auto msgF =  std_msgs::msg::Float64();
msgF.data = (count_/3.14);
RCLCPP_INFO(this->get_logger(), "Published message F: '%f'",msgF.data);
publisher_F->publish(msgF);

}

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MyNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}









