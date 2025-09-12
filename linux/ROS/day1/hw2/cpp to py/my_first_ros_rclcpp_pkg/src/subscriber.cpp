// #include "../include/my_first_ros_rclcpp_pkg/subscriber.hpp"

// MyNode::MyNode() : Node("s")
// {   
//    subS=this->create_subscription<std_msgs::msg::String>("tS",0,std::bind(&MyNode::topic_callbackS,this,std::placeholders::_1));
//    subI=this->create_subscription<std_msgs::msg::Int32>("tI",0,std::bind(&MyNode::topic_callbackI,this,std::placeholders::_1));
//    subF=this->create_subscription<std_msgs::msg::Float64>("tF",0,std::bind(&MyNode::topic_callbackF,this,std::placeholders::_1));
// }

// void MyNode::topic_callbackS(const std_msgs::msg::String::SharedPtr msgS) {
//     RCLCPP_INFO(this->get_logger(), "Published message S: '%s'",msgS->data.c_str());
// }
// void MyNode::topic_callbackI(const std_msgs::msg::Int32::SharedPtr msgI) {
//     RCLCPP_INFO(this->get_logger(), "Published message I: '%d'",msgI->data);
// }
// void MyNode::topic_callbackF(const std_msgs::msg::Float64::SharedPtr msgF) {
//     RCLCPP_INFO(this->get_logger(), "Published message F: '%f'",msgF->data);
// }

// int main(int argc, char ** argv)
// {
//     rclcpp::init(argc, argv);
//     auto node = std::make_shared<MyNode>();
//     rclcpp::spin(node);
//     rclcpp::shutdown();
//     return 0;
// }









