#include "ros2_worker.hpp"

RosWorker::RosWorker() : Node("ros2_only_node") {
  pub_ = this->create_publisher<std_msgs::msg::String>("ros2_topic", 10);

  sub_ = this->create_subscription<std_msgs::msg::String>(
    "ros2_topic", 10,
    [this](std_msgs::msg::String::SharedPtr msg) {
      RCLCPP_INFO(this->get_logger(), "[ROS2] received: %s", msg->data.c_str());
    });

  timer_ = this->create_wall_timer(
    std::chrono::milliseconds(100), [this]() {auto msg = std_msgs::msg::String();
      msg.data = "hello from ROS2"; pub_->publish(msg);});
}
