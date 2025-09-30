#include "ros2_worker.hpp"
#include "redis_worker.hpp"

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);

  RedisWorker redis_worker("test_channel");
	
	redis_worker.enableRateLimit(true);
	redis_worker.setPublishRateHz(10.0);

	std::thread redis_pub_thread([&redis_worker]() {
  int i = 0;
  while (rclcpp::ok()) {
    redis_worker.publish("hello from Redis " + std::to_string(i++));
  }
});

  auto node = std::make_shared<RosWorker>();
  rclcpp::spin(node);

  if (redis_pub_thread.joinable()) {
    redis_pub_thread.join();
  }
  rclcpp::shutdown();
  return 0;
}