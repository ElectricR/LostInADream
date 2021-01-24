#pragma once
#include <memory>

class DefaultWorld;

class Default {
public:
	Default(const std::shared_ptr<DefaultWorld>& default_world): 
		default_world_(default_world)
	{}

protected:
	std::shared_ptr<DefaultWorld> default_world_;
};
