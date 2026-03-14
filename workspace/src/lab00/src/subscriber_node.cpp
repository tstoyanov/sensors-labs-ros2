/** And in this file we just define the subscriber node */
#include <lab00/subscriber.hpp>

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
