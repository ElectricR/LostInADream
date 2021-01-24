#pragma once
#include <string>

namespace global {

static std::string path_to_res_directory = "/home/er/PROJECTS/Ornament/res/";

const double PI = 3.14159265;

enum class GameState {
	DEFAULT,
	MENU,
	METEOROIDS,
	SLAPAMOLE,
};

enum class Realm {
	DEFAULT,
	HUD,
	METEOROIDS,
	SLAPAMOLE,
};

}
