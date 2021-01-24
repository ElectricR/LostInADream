#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>


template <class T>
class RSS_Grid {
public:
	enum class RSS_GridType {
		DEFAULT,
		HUD,
		METEOROID,
	};

	RSS_Grid(int cell_size, RSS_GridType type):
		cell_size_(cell_size)
	{
		switch (type) {
		case RSS_GridType::DEFAULT: {
			grid_vertical_offset_ = 1080 / cell_size_ * 2;
			grid_horizontal_offset_ = 1920 / cell_size_ * 2;
			int verticalCellPerDisplaySizeRatio = 1080 / cell_size_;
			int horizontalCellPerDisplaySizeRatio = 1920 / cell_size_;
			int displayCount = 5;
			grid_ = std::vector<std::vector<std::vector<std::weak_ptr<T>>>>(
					verticalCellPerDisplaySizeRatio * displayCount, 
					std::vector<std::vector<std::weak_ptr<T>>>(
						horizontalCellPerDisplaySizeRatio * displayCount));
			break;
		}
		case RSS_GridType::METEOROID: {
			grid_vertical_offset_ = 7;
			grid_horizontal_offset_ = 0;

			int verticalCellPerDisplaySizeRatio = 1080 / cell_size_;
			int horizontalCellPerDisplaySizeRatio = 1920 / cell_size_;
			grid_ = std::vector<std::vector<std::vector<std::weak_ptr<T>>>>(
					verticalCellPerDisplaySizeRatio + 14, 
					std::vector<std::vector<std::weak_ptr<T>>>(
						horizontalCellPerDisplaySizeRatio));
			break;
		}
		case RSS_GridType::HUD: {
			grid_vertical_offset_ = 0;
			grid_horizontal_offset_ = 0;

			int verticalCellPerDisplaySizeRatio = 1080 / cell_size_;
			int horizontalCellPerDisplaySizeRatio = 1920 / cell_size_;
			grid_ = std::vector<std::vector<std::vector<std::weak_ptr<T>>>>(
					verticalCellPerDisplaySizeRatio, 
					std::vector<std::vector<std::weak_ptr<T>>>(
						horizontalCellPerDisplaySizeRatio));
			break;
		}
		}
	}

	int get_cell_size() {
		return this->cell_size_;
	}

	int get_horizontal_offset() {
		return this->grid_horizontal_offset_;
	}

	int get_vertical_offset() {
		return this->grid_vertical_offset_;
	}

	void insert(T* entity, int row, int col) {
		grid_[row][col].push_back(ptr_map_.at(entity));
	}

	void remove(T* entity, int row, int col) {
		for (size_t i = 0; i < grid_[row][col].size(); ++i) {
			if (grid_[row][col][i].lock().get() == entity) {
				grid_[row][col].erase(grid_[row][col].begin() + i);
				break;
			}
		}
	}

	void grid_dump() {
		for (size_t i = 0; i < grid_.size(); ++i) {
			for (size_t j = 0; j < grid_[i].size(); ++j) {
				if (!grid_[i][j].empty()) {
					std::cout << "Cell " << i << ' ' << j << " contains ids: ";
					for (size_t k = 0; k != grid_[i][j].size(); ++k) {
						std::cout << grid_[i][j][k].lock()->get_type_id() << ' ';
					}
					std::cout << std::endl;
				}
			}
		}
	}

	std::vector<std::weak_ptr<T>> &get_cell(int i, int j) {
		return grid_[i][j];
	}

	std::pair<int, int> get_grid_dims() {
		std::pair<int, int> dim_pair;
		dim_pair.first = grid_.size();
		dim_pair.second = grid_[0].size();
		return dim_pair;
	}

	void register_entity(std::weak_ptr<T> weak_ptr){
		ptr_map_[weak_ptr.lock().get()] = weak_ptr;
	}

private:
	int cell_size_;
	int grid_horizontal_offset_;
	int grid_vertical_offset_;
	std::vector<std::vector<std::vector<std::weak_ptr<T>>>> grid_;
	std::unordered_map<T*, std::weak_ptr<T>> ptr_map_;
};


