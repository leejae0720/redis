#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include "redis_worker.hpp"

// --------------------- ROS2 Node ---------------------
class RosWorker : public rclcpp::Node {
public:
	RosWorker() : Node("ros2_only_node") {
		pub_ = this->create_publisher<std_msgs::msg::String>("ros2_topic", 10);

		sub_ = this->create_subscription<std_msgs::msg::String>(
			"ros2_topic", 10,
			[this](std_msgs::msg::String::SharedPtr msg) {
					RCLCPP_INFO(this->get_logger(), "[ROS2] received: %s", msg->data.c_str());
			});

		timer_ = this->create_wall_timer(
			std::chrono::seconds(1),
			[this]() {
					auto msg = std_msgs::msg::String();
					msg.data = "hello from ROS2";
					pub_->publish(msg);
			});
	}

private:
	rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
	rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
	rclcpp::TimerBase::SharedPtr timer_;
};

// --------------------- Main ---------------------
int main(int argc, char* argv[]) {
	rclcpp::init(argc, argv);

	// Redis 시작 (별도 스레드에서 구독 실행됨)
	RedisWorker redis_worker("test_channel");

	// Redis에도 주기적으로 publish
	std::thread redis_pub_thread([&]() {
		for (int i = 0; i < 100; i++) {
			std::string msg = "hello from Redis " + std::to_string(i);
			redis_worker.publish(msg);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});

	// ROS2 노드 실행
	auto node = std::make_shared<RosWorker>();
	rclcpp::spin(node);

	if (redis_pub_thread.joinable()) redis_pub_thread.join();
	rclcpp::shutdown();
	return 0;
}
