#pragma once
#include <hiredis/hiredis.h>
#include <string>
#include <iostream>

redisContext* connectRedis(const std::string& host, int port) {
	redisContext* c = redisConnect(host.c_str(), port);
	if (c == nullptr || c->err) {
		if (c) {
			std::cerr << "Redis connection error: " << c->errstr << std::endl;
			redisFree(c);
		} else {
			std::cerr << "Connection allocation error\n";
		}
		return nullptr;
	}
	return c;
}
