#include "Renderer.h"

/*
static void meteoroid_render(const std::weak_ptr<Entity>& weak_entity) { //?????
	std::shared_ptr<Entity> entity = weak_entity.lock();
	if (entity->get_x() + entity->get_image_w() > 1920) { //?
		SDL_Rect frame_rect = SDL_Rect({ static_cast<int>(static_cast<int>(static_cast<int>(static_cast<int>(static_cast<int>(static_cast<int>(entity->get_frame())))))) * entity->get_image_w(), 0, entity->get_image_w(), entity->get_image_h() }); 
		SDL_Rect dest_rect = SDL_Rect({ static_cast<int>(entity->get_x()) - 1920, 
				static_cast<int>(entity->get_y()), 
				entity->get_image_w(), 
				entity->get_image_h() });
		SDL_RenderCopyEx(sdl_renderer, 
				&frame_rect,
				&dest_rect,
				entity->get_img(), // delete mtheod 
				entity->get_angle(), 
				NULL, 
				SDL_FLIP_NONE);
	}
}
*/

void Renderer::render() {
	SDL_RenderClear(sdl_renderer_);
	
	for (std::map<global::Realm, std::multiset<std::weak_ptr<Entity>, EntityLayerComparator>>::iterator realm_it = rend_list_.begin();
			realm_it != rend_list_.end(); 
			++realm_it){
		for (std::multiset<std::weak_ptr<Entity>, EntityLayerComparator>::iterator it = (*realm_it).second.begin();
				it != (*realm_it).second.end();
				++it) {
			(*it).lock()->render(sdl_renderer_, camera_, render_time_ratio_);
/*
			switch ((*it)->get_type_id()){
			case 13: {
				meteoroid_render((*it));
				break;
			}
			case 15: {
				meteoroid_render((*it));
				break;
			}
			}
			*/
		}
	}
	SDL_RenderPresent(sdl_renderer_);
}
