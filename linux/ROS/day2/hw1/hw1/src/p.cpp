#include <chrono>
#include <memory>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "tutorial_interfaces/msg/num.hpp"     // CHANGE

using namespace std::chrono_literals;

tutorial_interfaces::msg::Num data;

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher")
  {
    p = this->create_publisher<tutorial_interfaces::msg::Num>("topic", 10);    // CHANGE

    int i=0;
    
    std::cout<<"숫자 입력 ";
    std::cin>>i;
    if(!std::cin){
      std::cout<<" 입력 실패"<<std::endl;
      std::cin.clear();  
      std::cin.ignore(1000, '\n'); 
    }
    else{
      data.nv.push_back(i);
    }
    p->publish(data);
  }
private:

  rclcpp::Publisher<tutorial_interfaces::msg::Num>::SharedPtr p;         // CHANGE

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  while(1){
    MinimalPublisher();
  }
  rclcpp::shutdown();
  return 0;
}

//https://docs.ros.org/en/crystal/Tutorials/Custom-ROS2-Interfaces.html