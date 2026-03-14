#pragma once

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

/** It is good practice to split out more complex classes in separate files 
  * and libraries. This code follows the same tutorial as the publisher, but
  * separates classes into a library and executables in a separate file
  */

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber();
  private:
    void topic_callback(const std_msgs::msg::String & msg) const;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};
