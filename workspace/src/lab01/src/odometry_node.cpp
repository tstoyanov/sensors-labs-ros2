/** And in this file we just define the subscriber node */
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber();
  private:
    void topic_callback(const std_msgs::msg::String & msg) const;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

    
MinimalSubscriber::MinimalSubscriber()
: Node("minimal_subscriber")
{
  subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
}

void MinimalSubscriber::topic_callback(const std_msgs::msg::String & msg) const
{
  RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  // A different way to spin would be to create an executor.
  // This is sometimes useful when you want to switch to multi-threaded execution
  std::shared_ptr<rclcpp::Executor> executor = std::make_shared<rclcpp::executors::MultiThreadedExecutor>();

  auto sub_node_ = std::make_shared<MinimalSubscriber>();

  executor->add_node(sub_node_);
  executor->spin();
  
  rclcpp::shutdown();
  return 0;
}
