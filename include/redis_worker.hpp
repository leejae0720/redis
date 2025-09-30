#pragma once

#include <hiredis/hiredis.h>
#include <thread>
#include <atomic>
#include <iostream>

class RedisWorker {
public:
	RedisWorker(const std::string& channel)
		: channel_(channel), is_running_(true)
	{
		context_ = redisConnect("127.0.0.1", 6379);
		if (context_ == nullptr || context_->err) {
				throw std::runtime_error("Redis 연결 실패");
		}

		thread_ = std::thread(&RedisWorker::subscribeLoop, this);
	}

	~RedisWorker() {
		is_running_ = false;
		if (thread_.joinable()) thread_.join();
		if (context_) redisFree(context_);
	}

	void publish(const std::string& message) {
		redisCommand(context_, "PUBLISH %s %s", channel_.c_str(), message.c_str());
	}

private:
	void subscribeLoop() {
		redisContext* sub_ctx = redisConnect("127.0.0.1", 6379);
		redisReply* reply = (redisReply*)redisCommand(sub_ctx, "SUBSCRIBE %s", channel_.c_str());
		freeReplyObject(reply);

		while (is_running_) {
			redisReply* r;
			if (redisGetReply(sub_ctx, (void**)&r) == REDIS_OK && r != nullptr) {
				if (r->type == REDIS_REPLY_ARRAY && r->elements == 3) {
					std::string msg = r->element[2]->str;
					std::cout << "[Redis] received: " << msg << std::endl;
				}
				freeReplyObject(r);
			}
		}

		redisFree(sub_ctx);
	}

	std::string channel_;
	redisContext* context_;
	std::thread thread_;
	std::atomic<bool> is_running_;
};
