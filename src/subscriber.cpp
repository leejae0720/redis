#include "../include/redis_helper.hpp"
#include <iostream>

int main() {
  redisContext* c = connectRedis("127.0.0.1", 6379);
  if (!c) return -1;

  std::string channel = "test_channel";
  redisReply* reply;

  redisCommand(c, "SUBSCRIBE %s", channel.c_str());
  std::cout << "Subscribed to " << channel << std::endl;

  while (redisGetReply(c, (void**)&reply) == REDIS_OK) {
    if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 3) {
      std::string msg_type = reply->element[0]->str;
      std::string channel_name = reply->element[1]->str;
      std::string message = reply->element[2]->str;

      std::cout << "Received message from " << channel_name << ": " << message << std::endl;
    }
    freeReplyObject(reply);
  }

  redisFree(c);
  return 0;
}
