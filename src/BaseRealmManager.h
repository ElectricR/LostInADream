#pragma once
#include "Globals.h"
#include "Renderer.h"

class BaseRealmManager
{
public:
	BaseRealmManager(const std::shared_ptr<Renderer> &renderer, global::Realm realm):
		renderer_(renderer),
		realm_(realm)
	{
		renderer_->register_new_realm(realm_);
	}

	virtual ~BaseRealmManager() = default;
	BaseRealmManager(const BaseRealmManager&) = delete;
	BaseRealmManager& operator=(const BaseRealmManager&) = delete;
	
	virtual void manage_realm() = 0;
	virtual void create_realm() = 0;
	virtual void destroy_realm() = 0;
protected:
	std::shared_ptr<Renderer> renderer_;
	global::Realm realm_;
};

