#pragma once
#include <string>
#include <memory>

static inline bool operator<(std::pair<std::string, int> a, std::pair<std::string, int> b) {
	return a.second > b.second;
}

template<class T>
static inline bool operator<(const std::weak_ptr<T>& a, const std::weak_ptr<T>& b){
	return a.lock() < b.lock();
}

