/** And in this file we just define the subscriber node */
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

using std::placeholders::_1;

class OdometryNode : public rclcpp::Node
{
  public:
    OdometryNode();
  private:
    void topic_callback(const sensor_msgs::msg::JointState & msg) const;
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscription_;
    //TODO: Create a publisher that publishes nav_msgs/Odometry 
    //TODO: Create a timer that publishes odometry at a regular rate
    //TODO: Create members for internal model parameters
    //TODO: Create a subscriber that keeps track of the nav_msgs/Odometry published by the reference system
    //TODO: Create a service server triggered with an std_srvs/srv/Trigger and performs optimization of the model parameters. 
};

    
OdometryNode::OdometryNode()
: Node("minimal_subscriber")
{
  subscription_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "joint_state", 10, std::bind(&OdometryNode::topic_callback, this, _1));
}

void OdometryNode::topic_callback(const sensor_msgs::msg::JointState & msg) const
{
  //TODO: process JointStates and integrate into new vehicle state
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<OdometryNode>());
  
  rclcpp::shutdown();
  return 0;
}
