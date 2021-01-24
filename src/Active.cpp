#include <algorithm> 
#include "Active.h"


void Active::imprint_entity_to_grid() {
	std::shared_ptr<RSS_Grid<Active>> grid = default_grid_.lock(); 
	int start_x_cell = this->get_x() / grid->get_cell_size() + grid->get_horizontal_offset();
	int start_y_cell = this->get_y() / grid->get_cell_size() + grid->get_vertical_offset();
	if (this->get_x() < 0) { start_x_cell -= 1; }
	if (this->get_y() < 0) { start_y_cell -= 1; }
	int end_x_cell = (this->get_x() + this->get_image_w()) / grid->get_cell_size() + grid->get_horizontal_offset();
	int end_y_cell = (this->get_y() + this->get_image_h()) / grid->get_cell_size() + grid->get_vertical_offset();
	for (int i = start_x_cell; i <= end_x_cell; i++) {
		for (int j = start_y_cell; j <= end_y_cell; j++) {
			grid->insert(this, j, i);
		}
	}

}


void Active::remove_entity_from_grid() {
	std::shared_ptr<RSS_Grid<Active>> grid = default_grid_.lock(); 
	int start_x_cell = this->get_x() / grid->get_cell_size() + grid->get_horizontal_offset();
	int start_y_cell = this->get_y() / grid->get_cell_size() + grid->get_vertical_offset();
	if (this->get_x() < 0) { start_x_cell -= 1; }
	if (this->get_y() < 0) { start_y_cell -= 1; }
	int end_x_cell = (this->get_x() + this->get_image_w()) / grid->get_cell_size() + grid->get_horizontal_offset();
	int end_y_cell = (this->get_y() + this->get_image_h()) / grid->get_cell_size() + grid->get_vertical_offset();
	for (int i = start_x_cell; i <= end_x_cell; i++) {
		for (int j = start_y_cell; j <= end_y_cell; j++) {
			grid->remove(this, j, i);
		}
	}

}


ActionStatus Active::update_grid_position(Events::Event **event) {
	std::shared_ptr<RSS_Grid<Active>> grid = default_grid_.lock(); 
	int start_x_cell = this->get_x() / grid->get_cell_size() + grid->get_horizontal_offset();
	int start_y_cell = this->get_y() / grid->get_cell_size() + grid->get_vertical_offset();
	if (this->get_x() < 0) { start_x_cell -= 1; }
	if (this->get_y() < 0) { start_y_cell -= 1; }
	int end_x_cell = (this->get_x() + this->get_image_w()) / grid->get_cell_size() + 
		grid->get_horizontal_offset();
	int end_y_cell = (this->get_y() + this->get_image_h()) / grid->get_cell_size() + 
		grid->get_vertical_offset();
	
	if (start_x_cell < 0 || 
			start_y_cell < 0 || 
			end_y_cell > grid->get_grid_dims().first - 1 || 
			end_x_cell > grid->get_grid_dims().second - 1) 
	{
		return ActionStatus::END_OF_THE_LINE;
	}
	
	int old_start_x_cell = old_x_ / grid->get_cell_size() + grid->get_horizontal_offset();
	int old_start_y_cell = old_y_ / grid->get_cell_size() + grid->get_vertical_offset();
	if (old_x_ < 0) { old_start_x_cell -= 1; }
	if (old_y_ < 0) { old_start_y_cell -= 1; }
	int old_end_x_cell = (old_x_ + this->get_image_w()) / grid->get_cell_size() + 
		grid->get_horizontal_offset();
	int old_end_y_cell = (old_y_ + this->get_image_h()) / grid->get_cell_size() + 
		grid->get_vertical_offset();
	if (start_x_cell - old_start_x_cell > 0) { //left edge goes right
		for (int j = old_start_x_cell; j < start_x_cell; j++) {
			for (int i = old_start_y_cell; i <= old_end_y_cell; i++) {
				grid->remove(this, i, j);
			}
		}
	}
	else if (start_x_cell - old_start_x_cell < 0) {
		for (int j = start_x_cell; j < old_start_x_cell; j++) {
			for (int i = start_y_cell; i <= end_y_cell; i++) {
				grid->insert(this, i, j);
			}
		}
	}

	if (end_x_cell - old_end_x_cell > 0) {
		for (int j = old_end_x_cell + 1; j <= end_x_cell; j++) {
			for (int i = start_y_cell; i <= end_y_cell; i++) {
				grid->insert(this, i, j);
			}
		}
	}
	else if (end_x_cell - old_end_x_cell < 0) {
		for (int j = end_x_cell + 1; j <= old_end_x_cell; j++) {
			for (int i = old_start_y_cell; i <= old_end_y_cell; i++) {
				grid->remove(this, i, j);
			}
		}
	}

	if (start_y_cell - old_start_y_cell > 0) { //top edge goes bottom
		for (int i = old_start_y_cell; i < start_y_cell; i++) {
			for (int j = std::max(start_x_cell, old_start_x_cell); j <= std::min(end_x_cell, old_end_x_cell); j++) {
				grid->remove(this, i, j);
			}
		}
	}
	else if (start_y_cell - old_start_y_cell < 0) { //top edge goes top
		for (int i = start_y_cell; i < old_start_y_cell; i++) {
			for (int j = std::max(start_x_cell, old_start_x_cell); j <= std::min(end_x_cell, old_end_x_cell); j++) {
				grid->insert(this, i, j);
			}
		}
	}

	if (end_y_cell - old_end_y_cell > 0) { //bottom edge goes bottom
		for (int i = old_end_y_cell + 1; i <= end_y_cell; i++) {
			for (int j = std::max(start_x_cell, old_start_x_cell); j <= std::min(end_x_cell, old_end_x_cell); j++) {
				grid->insert(this, i, j);
			}
		}
	}
	else if (end_y_cell - old_end_y_cell < 0) { //bottom edge goes top
		for (int i = end_y_cell + 1; i <= old_end_y_cell; i++) {
			for (int j = std::max(start_x_cell, old_start_x_cell); j <= std::min(end_x_cell, old_end_x_cell); j++) {
				grid->remove(this, i, j);
			}
		}
	}
	this->establish_position();
	return ActionStatus::OK;
}

std::pair<std::pair<int, int>, std::pair<int, int>> Active::get_grid_cells() {
	std::shared_ptr<RSS_Grid<Active>> grid = default_grid_.lock(); 
	int start_x_cell = this->get_x() / grid->get_cell_size() + grid->get_horizontal_offset();
	int start_y_cell = this->get_y() / grid->get_cell_size() + grid->get_vertical_offset();
	if (this->get_x() < 0) { start_x_cell -= 1; }
	if (this->get_y() < 0) { start_y_cell -= 1; }
	int end_x_cell = (this->get_x() + this->get_image_w()) / grid->get_cell_size() + grid->get_horizontal_offset();
	int end_y_cell = (this->get_y() + this->get_image_h()) / grid->get_cell_size() + grid->get_vertical_offset();
	return std::pair<std::pair<int, int>, std::pair<int, int>> (
			std::pair<int, int>(start_y_cell, end_y_cell),
			std::pair<int,int>(start_x_cell, end_x_cell));
}

void Active::establish_position() {
	this->old_x_ = this->get_x();
	this->old_y_ = this->get_y();
}
