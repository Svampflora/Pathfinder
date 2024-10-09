#include "system.h"
#include <iostream>

namespace debug
{
    void info(std::string info)
    {
        std::cout << "info:" << info;
    }

    void warn(std::string warning)
    {
        std::cout << "warning:" << warning;
    }

    void error(std::string error)
    {
        std::cout << "error:" << error;
    }
}


namespace shared
{
	//struct Vector2i { int x, y; };

	Vector2 vector2_random(const Vector2& position, float distance)
	{
		float angle = (float)rand() / RAND_MAX * 2 * PI;
		float radius = (float)rand() / RAND_MAX * distance;
		Vector2 random_position = { position.x + radius * cos(angle), position.y + radius * sin(angle) };
		return random_position;
	}

	bool chance(int possibility_percentage)
	{
		int chance_in_a_hundred = rand() % 100;

		if (possibility_percentage > chance_in_a_hundred)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


}