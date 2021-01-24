#include "DefaultConstructor.h"
#include "Entity.h"
#include "TextureController.h"
#include <fstream>
#include <sstream>
#include "Player.h"
#include "SpiderSlime.h"


void DefaultConstructor::load_attrs(const std::string &name) {
	id_ = EntityType::NONE;
	scale_ = 0;
	layer_ = 0;

	std::ifstream input_file(global::path_to_res_directory + name + ".txt");
	std::string line;
	while (std::getline(input_file, line)) {
		std::istringstream temp_line(line);
		std::string p1;
		std::string p2;
		getline(temp_line, p1, ' ');
		getline(temp_line, p2, ' ');
		if (p1 == "id") {
			id_ = static_cast<EntityType>(stoi(p2)); 
		}
		else if (p1 == "scale") {
		       	scale_ = stoi(p2); 
		}
		else if (p1 == "layer") { 
			layer_ = stoi(p2); 
		}
		//else if (p1 == "total_frames") { this->total_frames = stoi(p2); }
		//else if (p1 == "new_stance") { std::string p3; getline(temp_line, p3, ' '); frame_info_container.push_back(pair<int, int>(stoi(p2), stoi(p3))); }
	}
}

Entity *DefaultConstructor::make_entity(const std::string &name, int x, int y) {
	load_attrs(name);
	SDL_Texture *img = tex_ctrl_->load_texture(name, scale_); 
	return new Entity(img, id_, layer_, x, y, scale_);
}
