#include "../include/redis_helper.hpp"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
  redisContext* c = connectRedis("127.0.0.1", 6379);
  if (!c) return -1;

  std::string channel = "test_channel";
  int counter = 1;

  std::cout << "Redis Auto Publisher Started (sending 1,2,3... every second)\n";

  while (true) {
    std::string message = std::to_string(counter++);

    redisReply* reply = (redisReply*)redisCommand(c, "PUBLISH %s %s", channel.c_str(), message.c_str());
    if (reply) {
      std::cout << "Published: " << message << std::endl;
      freeReplyObject(reply);
    } else {
      std::cerr << "Publish failed" << std::endl;
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));  // 1초 간격
  }

  redisFree(c);
  return 0;
}
