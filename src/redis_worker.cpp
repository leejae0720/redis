#include "redis_worker.hpp"

RedisWorker::RedisWorker(const std::string& channel)
    : channel_(channel), is_running_(true)
{
  context_ = redisConnect("127.0.0.1", 6379);
  if (!context_ || context_->err) {
    throw std::runtime_error("Redis connected fail");
  }
  thread_ = std::thread(&RedisWorker::subscribeLoop, this);
}

RedisWorker::~RedisWorker() {
  is_running_ = false;
  if (thread_.joinable()) thread_.join();
  if (context_) redisFree(context_);
}

void RedisWorker::enableRateLimit(bool on) {
  std::lock_guard<std::mutex> lk(rate_mtx_);
  rate_limit_on_ = on;
  if (!rate_limit_on_) {
    rate_hz_ = 0.0;
  }
}

void RedisWorker::setPublishRateHz(double hz) {
  std::lock_guard<std::mutex> lk(rate_mtx_);
  if (hz < 0.0) hz = 0.0;
  rate_hz_ = hz;
  if (next_send_ < std::chrono::steady_clock::now()) {
    next_send_ = std::chrono::steady_clock::now();
  }
}

void RedisWorker::publish(const std::string& message) {
    std::unique_lock<std::mutex> lk(rate_mtx_);

    if (rate_limit_on_ && rate_hz_ > 0.0) {
      const auto period = std::chrono::duration<double>(1.0 / rate_hz_);
      const auto step   = std::chrono::duration_cast<std::chrono::steady_clock::duration>(period);

      auto now = std::chrono::steady_clock::now();
      if (now < next_send_) {
        std::this_thread::sleep_until(next_send_);
        now = std::chrono::steady_clock::now();
      }
      next_send_ = now + step;
    }
    redisReply* reply = (redisReply*)redisCommand(context_, "PUBLISH %s %s",
                                                  channel_.c_str(), message.c_str());
    if (reply) freeReplyObject(reply);

    lk.unlock();
}

void RedisWorker::subscribeLoop() {
  redisContext* sub_ctx = redisConnect("127.0.0.1", 6379);
  if (!sub_ctx || sub_ctx->err) {
    std::cerr << "Redis SUBSCRIBE connected fail\n";
    return;
  }

  redisReply* reply = (redisReply*)redisCommand(sub_ctx, "SUBSCRIBE %s", channel_.c_str());
  if (reply) freeReplyObject(reply);

  while (is_running_) {
    redisReply* r = nullptr;
    if (redisGetReply(sub_ctx, (void**)&r) == REDIS_OK && r) {
      if (r->type == REDIS_REPLY_ARRAY && r->elements == 3) {
        const char* s = r->element[2]->str;
        std::cout << "[Redis] received: " << (s ? s : "") << std::endl;
      }
      freeReplyObject(r);
    }
  }
  redisFree(sub_ctx);
}
