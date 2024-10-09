#include "level.h"
#include "system.h"



void Level::reset()
{

	grid.clear();
	vacant_tiles.clear();
	grid_dimensions = { 20, 20 };
	tile_size = { (float)GetScreenWidth() / grid_dimensions.x, (float)GetScreenHeight() / grid_dimensions.y };

	for (int y = 0; y < grid_dimensions.y; y++)
	{
		for (int x = 0; x < grid_dimensions.x; x++)
		{

			Tile _tile;
			_tile.position = { tile_size.x * x, tile_size.y * y };
			_tile.vacant = rand() > (RAND_MAX / 2);
			grid.push_back(_tile);

			if (_tile.vacant)
			{
				int index = (int)grid.size() - 1;
				Tile* vacant_tile = std::addressof(grid[index]);
				vacant_tiles.push_back(vacant_tile);
				
			}
		}
	}

	int random_vacant_tile = rand() % vacant_tiles.size();

	if (!fallen_star.create("assets/cheese.png", vacant_tiles[random_vacant_tile]))
	{
		debug::warn("fallen_star was not created");
	}

	random_vacant_tile = rand() % vacant_tiles.size();

	if (!trading_post.create("assets/family.png", vacant_tiles[random_vacant_tile]))
	{
		debug::warn("trading_post was not created");
	}

	random_vacant_tile = rand() % vacant_tiles.size();

	if (!spaceship.create("assets/boot.png", vacant_tiles[random_vacant_tile]))
	{
		debug::warn("spaceship was not created");
	}

	random_vacant_tile = rand() % vacant_tiles.size();

	if (!starchaser.create("assets/mouse.png", vacant_tiles[random_vacant_tile]))
	{
		debug::warn("starchaser was not created");
	}
	starchaser.sense(this);
}

void Level::update()
{

	grid_interaction();
	starchaser.update(this);
	star_carrying();
}

void Level::draw()
{
	for (Tile _tile : grid)
	{
		if (_tile.vacant)
		{
			DrawRectangle(_tile.position.x, _tile.position.y, tile_size.x, tile_size.y, BLACK);
		}
		else
		{
			DrawRectangle(_tile.position.x, _tile.position.y, tile_size.x, tile_size.y, WHITE);
		}
	}

	fallen_star.draw(tile_size);
	trading_post.draw(tile_size);
	spaceship.draw(tile_size);
	starchaser.draw_pathfinder(tile_size);
	starchaser.draw(tile_size);
}

void Level:: grid_interaction()
{
	Rectangle button;

	entity_selection(&starchaser);
	entity_selection(&trading_post);
	entity_selection(&spaceship);
	entity_selection(&fallen_star);
	
	starchaser.move(GetMousePosition());
	trading_post.move(GetMousePosition());
	spaceship.move(GetMousePosition());
	fallen_star.move(GetMousePosition());

	if (IsMouseButtonReleased(MouseButton::MOUSE_BUTTON_LEFT))
	{
		for (int i = 0; i < grid.size(); i++)
		{
			button = { grid[i].position.x, grid[i].position.y, tile_size.x, tile_size.y};
			if (CheckCollisionPointRec(GetMousePosition(), button))
			{
				if (CheckCollisionPointRec(GetMousePosition(), starchaser.get_collision()) && grid[i].vacant)
				{
					starchaser.new_home(&grid[i]);
				}
				else if (CheckCollisionPointRec(GetMousePosition(), trading_post.get_collision()) && grid[i].vacant)
				{
					trading_post.new_home(&grid[i]);
				}
				else if (CheckCollisionPointRec(GetMousePosition(), spaceship.get_collision()) && grid[i].vacant)
				{
					spaceship.new_home(&grid[i]);
				}
				else if (CheckCollisionPointRec(GetMousePosition(), fallen_star.get_collision()) && grid[i].vacant)
				{
					fallen_star.new_home(&grid[i]);
				}
				else if (grid[i].vacant)
				{
					grid[i].vacant = false;
				}
				else
				{
					grid[i].vacant = true;
				}
			}
		}
		starchaser.sense(this);
	}

}
	void Level::entity_selection(Entity* entity)
	{
		if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
		{
			if (CheckCollisionPointRec(GetMousePosition(), entity->get_collision()))
			{
				entity->select();
			}
		}
		else entity->deselect();
	}

	void Level::star_carrying()
	{
		if (starchaser.get_doing() == Starchaser::star_transporting )
		{
			fallen_star.new_home(starchaser.get_current_tile(this));
		}

		if (fallen_star.get_current_tile(this) == trading_post.get_current_tile(this))
		{
			int random_vacant_tile = rand() % vacant_tiles.size();
			fallen_star.new_home(vacant_tiles[random_vacant_tile]);
		}
	}

	bool Level::check_tile(const shared::Vector2i index) const
	{

		bool vacant = grid[index.y * grid_dimensions.x + index.x].vacant;
		return vacant;
	}

	Tile* Level::get_tile(const shared::Vector2i index)
	{
		return  std::addressof(grid[index.y * grid_dimensions.x + index.x]);
	}

	int Level::get_tile_index(const shared::Vector2i index) const
	{

		int list_index = index.y * grid_dimensions.x + index.x;
		return list_index;
	}

	shared::Vector2i Level::get_2d_index(Vector2 _position) const
	{
		shared::Vector2i d_index = { _position.x / tile_size.x, _position.y / tile_size.y};
		return d_index;
	}

	Vector2 Level::get_tile_position(int index)const
	{
		Vector2 position = grid[index].position;

		return position;
	}

	Vector2 Level::get_tile_size()const
	{
		return tile_size;
	}

	shared::Vector2i Level::get_grid_dimensions() const
	{

		return grid_dimensions;
	}

	Tile* Level::get_fallen_star_tile()
	{
		return fallen_star.get_home();
	}

	Tile* Level::get_spaceship_tile()
	{
		return spaceship.get_home();
	}

	Tile* Level::get_trading_post_tile()
	{
		return trading_post.get_home();
	}