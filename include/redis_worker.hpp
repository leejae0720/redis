#pragma once

#include <hiredis/hiredis.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <string>
#include <mutex>
#include <chrono>

class RedisWorker {
	public:
		explicit RedisWorker(const std::string& channel);
		~RedisWorker();

		void publish(const std::string& message);

    void enableRateLimit(bool on);
    void setPublishRateHz(double hz);

	private:
		void subscribeLoop();

		std::string channel_;
		redisContext* context_{nullptr};
		std::thread thread_;
		std::atomic<bool> is_running_{false};

		std::mutex rate_mtx_;
    bool rate_limit_on_{false};
    double rate_hz_{0.0};
    std::chrono::steady_clock::time_point next_send_{std::chrono::steady_clock::now()};
};
