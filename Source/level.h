#pragma once

#include <list>
#include <deque>
#include "entity.h"

class Level;



	
class Level 
{
	Starchaser starchaser;
	Entity fallen_star;
	Entity trading_post;
	Entity spaceship;
	std::deque<Tile> grid;
	std::vector<Tile*> vacant_tiles;
	Vector2 tile_size;
	shared::Vector2i grid_dimensions;

	void grid_interaction();
	void entity_selection(Entity* entity);
	void star_carrying();
public:

	void reset();
	void update();
	void draw();
	bool check_tile(const shared::Vector2i index) const;
	int get_tile_index(const shared::Vector2i index) const;
	Vector2 get_tile_position(int index)const;
	Vector2 get_tile_size()const;
	shared::Vector2i get_grid_dimensions() const;
	shared::Vector2i get_2d_index(Vector2 position) const;
	Tile* get_tile(const shared::Vector2i);
	Tile* get_fallen_star_tile();
	Tile* get_spaceship_tile();
	Tile* get_trading_post_tile();
};