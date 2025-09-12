// #include "rclcpp/rclcpp.hpp"
// #include "std_msgs/msg/string.hpp"


// class MyCppNode : public rclcpp::Node{
//     public:
//     MyCppNode() : Node("my_cpp_node"){
//         RCLCPP_INFO(this->get_logger(),"Hello,ROS 2 c++ Node!");
//     }
// };


// class MyNode : public rclcpp::Node
// {
//     public:
//     MyNode();
//     ~MyNode();
//     private:
//     rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;

// };
// int main(int argc, char **argv){
//     rclcpp::init(argc,argv);
//     auto node = std::make_shared<MyCppNode>();
//     rclcpp::spin(node);
//     rclcpp::shutdown();
//     return 0;    
// }


// MyNode::MyNode() : Node("mynode"){
//     publisher = this->create_publisher<std_msgs::msg::String>("topicname",10);
// }
// MyNode::~MyNode(){
    
// }