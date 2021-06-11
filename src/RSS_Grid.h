#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <SDL2/SDL.h>

class CollideComponent;

template <class T>
class RSS_Grid {
public:
	RSS_Grid(int cell_size):
		cell_size_(cell_size)
	{
        if constexpr (std::is_same_v<T, CollideComponent>) {
			vertical_offset_ = 1080 / cell_size_ * 2;
			horizontal_offset_ = 1920 / cell_size_ * 2;
			int verticalCellPerDisplaySizeRatio = 1080 / cell_size_;
			int horizontalCellPerDisplaySizeRatio = 1920 / cell_size_;
			int displayCount = 5;
			grid_ = std::vector<std::vector<std::vector<std::shared_ptr<T>>>>(
					verticalCellPerDisplaySizeRatio * displayCount, 
					std::vector<std::vector<std::shared_ptr<T>>>(
						horizontalCellPerDisplaySizeRatio * displayCount));
		}
        // TODO
        /*
		case RSS_GridType::METEOROID: {
			grid_vertical_offset_ = 7;
			grid_horizontal_offset_ = 0;

			int verticalCellPerDisplaySizeRatio = 1080 / cell_size_;
			int horizontalCellPerDisplaySizeRatio = 1920 / cell_size_;
			grid_ = std::vector<std::vector<std::vector<std::shared_ptr<T>>>>(
					verticalCellPerDisplaySizeRatio + 14, 
					std::vector<std::vector<std::shared_ptr<T>>>(
						horizontalCellPerDisplaySizeRatio));
			break;
		}
		case RSS_GridType::HUD: {
			grid_vertical_offset_ = 0;
			grid_horizontal_offset_ = 0;

			int verticalCellPerDisplaySizeRatio = 1080 / cell_size_;
			int horizontalCellPerDisplaySizeRatio = 1920 / cell_size_;
			grid_ = std::vector<std::vector<std::vector<std::shared_ptr<T>>>>(
					verticalCellPerDisplaySizeRatio, 
					std::vector<std::vector<std::shared_ptr<T>>>(
						horizontalCellPerDisplaySizeRatio));
			break;
		}
        */
	}

	void track(const std::shared_ptr<T>& component) {
        int start_x_cell = component->start_x / cell_size_ + horizontal_offset_;
        int start_y_cell = component->start_y / cell_size_ + vertical_offset_;
        if (component->start_x < 0) { start_x_cell -= 1; }
        if (component->start_y < 0) { start_y_cell -= 1; }
        int end_x_cell = (component->start_x + component->width) / cell_size_ + horizontal_offset_;
        int end_y_cell = (component->start_y + component->height) / cell_size_ + vertical_offset_;
        for (int x = start_x_cell; x <= end_x_cell; ++x) {
            for (int y = start_y_cell; y <= end_y_cell; ++y) {
                grid_[x][y].push_back(component);
            }
        }
    }

    void update(const std::shared_ptr<T>& component) {

        component->update();

        int start_x_cell = component->start_x / cell_size_ + horizontal_offset_;
        int start_y_cell = component->start_y / cell_size_ + vertical_offset_;
        if (component->start_x < 0) { start_x_cell -= 1; }
        if (component->start_y < 0) { start_y_cell -= 1; }
        int end_x_cell = (component->start_x + component->width) / cell_size_ + horizontal_offset_;
        int end_y_cell = (component->start_y + component->height) / cell_size_ + vertical_offset_;
        
        int old_start_x_cell = component->old_start_x / cell_size_ + horizontal_offset_;
        int old_start_y_cell = component->old_start_y / cell_size_ + vertical_offset_;
        if (old_start_x_cell < 0) { old_start_x_cell -= 1; }
        if (old_start_y_cell < 0) { old_start_y_cell -= 1; }
        int old_end_x_cell = (component->old_start_x + component->width) / cell_size_ + 
            horizontal_offset_;
        int old_end_y_cell = (component->old_start_y + component->height) / cell_size_ + 
            vertical_offset_;

        if (start_x_cell - old_start_x_cell > 0) { //left edge goes right
            for (int x = old_start_x_cell; x < start_x_cell; ++x) {
                for (int y = old_start_y_cell; y <= old_end_y_cell; ++y) {
                    this->remove(component, x, y);
                }
            }
        }
        else if (start_x_cell - old_start_x_cell < 0) {
            for (int x = start_x_cell; x < old_start_x_cell; ++x) {
                for (int y = start_y_cell; y <= end_y_cell; ++y) {
                    this->insert(component, x, y);
                }
            }
        }

        if (end_x_cell - old_end_x_cell > 0) {
            for (int x = old_end_x_cell + 1; x <= end_x_cell; ++x) {
                for (int y = start_y_cell; y <= end_y_cell; ++y) {
                    this->insert(component, x, y);
                }
            }
        }
        else if (end_x_cell - old_end_x_cell < 0) {
            for (int x = end_x_cell + 1; x <= old_end_x_cell; x++) {
                for (int y = old_start_y_cell; y <= old_end_y_cell; ++y) {
                    this->remove(component, x, y);
                }
            }
        }

        if (start_y_cell - old_start_y_cell > 0) { //top edge goes bottom
            for (int x = std::max(start_x_cell, old_start_x_cell); x <= std::min(end_x_cell, old_end_x_cell); ++x) {
                for (int y = old_start_y_cell; y < start_y_cell; ++y) {
                    this->remove(component, x, y);
                }
            }
        }
        else if (start_y_cell - old_start_y_cell < 0) { //top edge goes top
            for (int x = std::max(start_x_cell, old_start_x_cell); x <= std::min(end_x_cell, old_end_x_cell); ++x) {
                for (int y = start_y_cell; y < old_start_y_cell; ++y) {
                    this->insert(component, x, y);
                }
            }
        }

        if (end_y_cell - old_end_y_cell > 0) { //bottom edge goes bottom
            for (int x = std::max(start_x_cell, old_start_x_cell); x <= std::min(end_x_cell, old_end_x_cell); ++x) {
                for (int y = old_end_y_cell + 1; y <= end_y_cell; ++y) {
                    this->insert(component, x, y);
                }
            }
        }
        else if (end_y_cell - old_end_y_cell < 0) { //bottom edge goes top
            for (int x = std::max(start_x_cell, old_start_x_cell); x <= std::min(end_x_cell, old_end_x_cell); ++x) {
                for (int y = end_y_cell + 1; y <= old_end_y_cell; ++y) {
                    this->remove(component, x, y);
                }
            }
        }
        component->establish();
    }

    void untrack(const std::shared_ptr<T>& component) {
        int start_x_cell = component->start_x / cell_size_ + horizontal_offset_;
        int start_y_cell = component->start_y / cell_size_ + vertical_offset_;
        if (component->start_x < 0) { start_x_cell -= 1; }
        if (component->start_y < 0) { start_y_cell -= 1; }
        int end_x_cell = (component->start_x + component->width) / cell_size_ + horizontal_offset_;
        int end_y_cell = (component->start_y + component->height) / cell_size_ + vertical_offset_;

        for (int x = start_x_cell; x <= end_x_cell; ++x) {
            for (int y = start_y_cell; y <= end_y_cell; ++y) {
                this->remove(component, x, y);
            }
        }
    }

    std::unordered_map<std::shared_ptr<T>, SDL_Rect> get_collisions(const std::shared_ptr<T>& component) {
        int start_x_cell = component->start_x / cell_size_ + horizontal_offset_;
        int start_y_cell = component->start_y / cell_size_ + vertical_offset_;
        if (component->start_x < 0) { start_x_cell -= 1; }
        if (component->start_y < 0) { start_y_cell -= 1; }
        int end_x_cell = (component->start_x + component->width) / cell_size_ + horizontal_offset_;
        int end_y_cell = (component->start_y + component->height) / cell_size_ + vertical_offset_;
        std::unordered_map<std::shared_ptr<T>, SDL_Rect> ret;

        for (int x = start_x_cell; x != end_x_cell + 1; ++x) {
            for (int y = start_y_cell; y != end_y_cell + 1; ++y) {
                for (auto& tracked : grid_[x][y]) {
                    if (tracked->entity != component->entity) {
                        SDL_Rect collision_rect;
                        if (this->check_collision(component, tracked, &collision_rect)) {
                            ret[tracked] = collision_rect;
                        } 
                    }
                }
            }
        }
        return ret;
    }

private:

    bool check_collision(const std::shared_ptr<T>& first, const std::shared_ptr<T>& second, SDL_Rect *collision_rect) const {
        SDL_Rect A {static_cast<int>(first->start_x), static_cast<int>(first->start_y), first->width, first->height };
        SDL_Rect B {static_cast<int>(second->start_x), static_cast<int>(second->start_y), second->width, second->height };

        if (SDL_IntersectRect(&A, &B, collision_rect)) {
            return true;
        }
        return false;
    }

    void insert(const std::shared_ptr<T>& component, int x, int y) {
		grid_[x][y].push_back(component);
	}

    void remove(const std::shared_ptr<T>& component, int x, int y) {
		for (size_t i = 0; i < grid_[x][y].size(); ++i) {
			if (grid_[x][y][i] == component) {
				grid_[x][y].erase(grid_[x][y].begin() + i);
				break;
			}
		}
	}

    int cell_size_;
    int horizontal_offset_;
    int vertical_offset_;
    std::vector<std::vector<std::vector<std::shared_ptr<T>>>> grid_;
};


