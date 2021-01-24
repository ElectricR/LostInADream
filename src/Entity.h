#pragma once
#include <SDL2/SDL.h>
#include "Globals.h"
#include "EntityTypes.h"
#include <string>
#include <memory>

struct DefaultIntentionFlagCollection;
class Camera;

class Entity {
public:
	Entity(SDL_Texture *img, EntityType id, int layer, int x, int y, int scale);
	virtual ~Entity() = default;
	
	void render(SDL_Renderer *renderer);
	void render(SDL_Renderer *renderer, std::shared_ptr<Camera> c, double render_time_ratio);

	void set_x(double value) {
		x_ = value;
	}

	void shift_x(double value){
		x_ += value;
	}

	void shift_y(double value){
		y_ += value;
	}

	double get_x() {
		return x_;
	}

	void set_y(double value) {
		y_ = value;
	}

	double get_y() {
		return y_;
	}

	void set_image_h(int value) {
		image_h_ = value;
	}

	int get_image_h() {
		return image_h_;
	}

	void set_image_w(int value) {
		image_w_ = value;
	}

	int get_image_w() {
		return image_w_;
	}

	void set_angle(double value) {
		angle_ = value;
		angle_r_ = value / 180 * global::PI;
	}

	void rotate(double value) {
		angle_ += value;
		angle_r_ = angle_ / 180 * global::PI;
	}

	double get_angle() {
		return angle_;
	}

	double get_angle_r() {
		return angle_r_;
	}

	EntityType get_type_id() {
		return id_;
	}

	double get_vspeed() {
		return vspeed_;
	}

	void set_vspeed(double value) {
		vspeed_ = value;
	}

	void add_vspeed(double value) {
		vspeed_ += value;
	}

	double get_hspeed() {
		return hspeed_;
	}

	void set_hspeed(double value) {
		hspeed_ = value;
	}

	void add_hspeed(double value) {
		hspeed_ += value;
	}

	int get_layer() const {
		return layer_;
	}

	void set_default_intention_flag_collection(
			const std::shared_ptr<DefaultIntentionFlagCollection>& ptr) {
		DIFC_ = ptr;
		under_control_ = true;
	}

	unsigned int get_frame() {
		return frame_;
	}

	double get_center_x() {
		return x_ + image_w_ / 2;
	}

	double get_center_y() {
		return y_ + image_h_ / 2;
	}


private:
	SDL_Texture *img_;
	EntityType id_ = EntityType::NONE;
	double x_ = 0;
	double y_ = 0;
	int image_w_ = 0;
	int image_h_ = 0;
	double angle_ = 0;
	double angle_r_ = 0;
	double vspeed_ = 0;
	double hspeed_ = 0;
	int layer_ = 0;
	int scale_ = 1;
	unsigned int frame_ = 0;
	unsigned int total_frames_ = 0;
protected:
	std::shared_ptr<DefaultIntentionFlagCollection> DIFC_;
	bool under_control_ = false;
};

class EntityLayerComparator {
public:
	bool operator()(const std::weak_ptr<Entity>& a, const std::weak_ptr<Entity>& b) const {
		return a.lock()->get_layer() < b.lock()->get_layer();
	}
};
