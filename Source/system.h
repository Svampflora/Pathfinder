#pragma once
#include <string>
#include "raylib.h"
//#include <vector>

namespace debug
{
	void info(std::string info);
	void warn(std::string warning);
	void error(std::string error);
} // !debug

namespace shared
{
	Vector2 vector2_random(const Vector2& position, float distance);
	bool chance(int possibility_percentage);

	struct Vector2i { int x, y; };



}