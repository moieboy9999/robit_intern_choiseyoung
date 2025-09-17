// #include "rclcpp/rclcpp.hpp"
// #include "std_msgs/msg/string.hpp"
// #include "lifecycle_node.hpp"

// #include <chrono>
// #include <iostream>
// #include <memory>
// #include <string>
// #include <thread>
// #include <utility>

// class MyNode : public rclcpp_lifecycle::LifecycleNode
// {
//     public:
//     MyNode();



//     private:
    
//     rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
//     on_configure(const rclcpp_lifecycle::State & previous_state)

//     rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
//     on_activate(const rclcpp_lifecycle::State & previous_state)

//     rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
//     on_deactivate(const rclcpp_lifecycle::State & previous_state)

//     rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
//     on_cleanup(const rclcpp_lifecycle::State & previous_state)

//     rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
//     on_shutdown(const rclcpp_lifecycle::State & previous_state)

//     rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subS;


//     void topic_callbackS(const std_msgs::msg::String::SharedPtr msgS);


// };