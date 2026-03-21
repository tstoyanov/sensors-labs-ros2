/** And in this file we just define the subscriber node */
#include "rclcpp/rclcpp.hpp"
#include "lab01/msg/encoder.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

using std::placeholders::_1;

class EncoderSubscriber : public rclcpp::Node
{
  public:
    EncoderSubscriber();
  private:
    void topic_callback(const lab01::msg::Encoder & msg) const;
    rclcpp::Subscription<lab01::msg::Encoder>::SharedPtr subscription_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
};

    
EncoderSubscriber::EncoderSubscriber()
: Node("encoder_subscriber")
{
  subscription_ = this->create_subscription<lab01::msg::Encoder>(
      "ticks", 10, std::bind(&EncoderSubscriber::topic_callback, this, _1));
  publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("wheel_state", 10);
}

void EncoderSubscriber::topic_callback(const lab01::msg::Encoder & msg) const
{
  //TODO
  //Here you need to handle the new encoder message
  //1. Is the wheel moving in positive or negative direction?
  //2. Update the ticks for position estimation
  //3. Calculate the velocity (hint: use finite differences and a timer
  //4. Convert to radian and radian per second
  //5. Fill the data in the joint state message and publish

}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  //create executor and spin
  std::shared_ptr<rclcpp::Executor> executor = std::make_shared<rclcpp::executors::MultiThreadedExecutor>();

  auto sub_node_ = std::make_shared<EncoderSubscriber>();

  executor->add_node(sub_node_);
  executor->spin();
  
  rclcpp::shutdown();
  return 0;
}
