#include "entity.h"
#include "level.h"

bool Entity::create(const char* texture_path, Tile* home)
{
	sprite = LoadTexture(texture_path);
	sprite.height = 40;
	sprite.width = 40;
	tint = {255, 255, 255, 255};
	home_tile = home;
	position = home->position;

	return true;
}

Rectangle Entity::get_collision()
{
	Rectangle collision_rec = {position.x, position.y, (float)sprite.width, (float)sprite.height};
	return collision_rec;

}

void Entity::select()
{
	selected = true;
}

void Entity::deselect()
{
	selected = false;
}

void Entity::move(Vector2 new_position)
{
	if (selected)
	{
		position = { new_position.x - (float)(0.5 * sprite.width), new_position.y - (float)(0.5 * sprite.height) };

	}
}

void Entity::new_home(Tile* home)
{
	home_tile = home;
	position = home_tile->position;

}

Tile* Entity::get_home()
{
	return home_tile;
}

Tile* Entity::get_current_tile(Level* level)
{
	shared::Vector2i current_index = level->get_2d_index(position);
	return level->get_tile(current_index);
}

void Entity::draw(Vector2 tile_size)
{

	DrawTexture(sprite, position.x, position.y, tint);
}

bool Starchaser::create(const char* texture_path, Tile* home)
{
	Entity::create(texture_path, home);
	max_stamina = 80;
	sense_timer = 0;
	stamina = max_stamina;
	action = star_hunting;
	pathfinder.create();


	return true;
}

void Starchaser::sense(Level* level)
{
	switch (action)
	{
	case star_hunting:
	{
		path = pathfinder.get_path(level, level->get_fallen_star_tile(), get_current_tile(level));

		break;
	}
	case star_transporting:
	{

		path = pathfinder.get_path(level, level->get_trading_post_tile(), get_current_tile(level));

		break;
	}
	case retrieting:
	{

		path = pathfinder.get_path(level, level->get_spaceship_tile(), get_current_tile(level));

		break;
	}
	case resting:
	{
		path = pathfinder.get_path(level, level->get_spaceship_tile(), get_current_tile(level));
		break;
	}
	}

}

void Starchaser::decide()
{

	switch (action)
	{
	case star_hunting: 
	{

		if (stamina < 1)
		{
			path.clear();
			action = state::retrieting;
			
		}
		else if (path.size() < 2)
		{
			action = state::star_transporting;
		}
		break;
	}
	case star_transporting: 
	{

		if (path.size() < 2)
		{
			action = state::star_hunting;
		}
		else if (stamina < 1)
		{
			path.clear();
			action = state::retrieting;
		}

		break;
	}
	case retrieting: 
	{

		if (path.size() < 2)
		{
			action = state::resting;
		}

		break;
	}
	case resting: 
	{
		if (stamina >= max_stamina)
		{
			action = state::star_hunting;
		}
		else if (stamina < max_stamina && path.size() > 1)
		{
			action = state::retrieting;
		}

		break;
	}
	}
}

void Starchaser::act(Level* level)
{
	if (path.size() > 0)
	{
		path.pop_back();
	}

	switch (action)
	{
	case star_hunting:
	{
		if (path.size() > 0)
		{
			position = path[path.size() - 1].tile->position;
			
		}
		else
		{
			sense(level);
		}

		stamina -= 0.1 * GetFrameTime();
		break;
	}
	case star_transporting:
	{
		if (path.size() > 0)
		{
			position = path[path.size() - 1].tile->position;

		}
		else
		{
			sense(level);
		}

		stamina -= 0.1 * GetFrameTime();
		break;
	}
	case retrieting:
	{
		
		if (path.size() > 0)
		{
			
			position = path[path.size() - 1].tile->position;

		}
		else
		{
			sense(level);
		}

		break;
	}
	case resting:
	{
		//float added_stamina = 0.1 * GetFrameTime();
		//stamina += added_stamina;
		stamina = max_stamina;
		break;
	}
	}

}

void Starchaser::update(Level* level)
{
	if (path.size() == 0)
	{
		sense(level);
	}

	if (timer(10))
	{
		decide();
		act(level);

	}
}

bool Starchaser::timer(int frequency)
{
	sense_timer--;
	if (sense_timer > 0)
	{
		return false;
	}
	sense_timer = frequency;
	return true;
}

void Starchaser::draw_pathfinder(Vector2 tile_size)
{
	pathfinder.draw(tile_size);
	info.draw(this);
}

int Starchaser::get_stamina() const
{
	return stamina;
}

Starchaser::state Starchaser::get_doing() const
{
	return action;
}