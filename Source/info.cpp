#include "info.h"
#include "entity.h"

void Info::draw(const Starchaser* chaser)
{
	Vector2 bar_pos = { 10,10 };
	Vector2 bar_size = { 100, 10 };
	float stamina = chaser->get_stamina() * 0.01f * bar_size.x;


	DrawRectangle((int)bar_pos.x, (int)bar_pos.y, (int)bar_size.x, (int)bar_size.y, RED);
	DrawRectangle((int)bar_pos.x, (int)bar_pos.y, (int)stamina, (int)bar_size.y, GREEN);
	DrawText("stamina", (int)bar_pos.x, (int)bar_pos.y, 10, WHITE);

	bar_pos.y += 20;

	std::string doing;

	switch (chaser->get_doing())
	{
	case Starchaser::state::star_hunting:
		doing = "star hunting";
		break;
	case Starchaser::state::star_transporting:
		doing = "star transporting";
		break;
	case Starchaser::retrieting:
		doing = "retrieting";
		break;
	case Starchaser::state::resting:
		doing = "resting";
		break;
	}
	const char* c_message = doing.c_str();
	DrawText(c_message, (int)bar_pos.x, (int)bar_pos.y, 10, WHITE);

}