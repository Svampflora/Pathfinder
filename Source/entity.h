#pragma once
#include "raylib.h"
#include "pathfinding.h"
#include "system.h"
#include "info.h"


class Level;

struct Tile
{
	Vector2 position;
	bool vacant;
};

class Entity
{
	Texture sprite;
	Color tint;
	Tile* home_tile;
	bool selected;
protected:
	Vector2 position;
public:

	bool create(const char* texture_path, Tile* home);
	void select();
	void deselect();
	void draw(Vector2 tile_size);
	Rectangle get_collision();
	void move(Vector2 new_position);
	void new_home(Tile* home);
	Tile* get_home();
	Tile* get_current_tile(Level* level);

};

class Starchaser: public Entity
{
public:
	enum state  {star_hunting, star_transporting, retrieting, resting};

private:
	Pathfinder pathfinder;
	Info info;
	std::deque<Tile_node> path;
	state action;
	int max_stamina;
	int stamina;
	int sense_timer;
	bool have_a_star;
	bool is_home;

public:

	bool create(const char* texture_path, Tile* home);
	void sense(Level* level);
	void decide();
	void act(Level* level);
	void update(Level* level);
	void draw_pathfinder(Vector2 tile_size);
	bool timer(int frequency);

	int get_stamina() const;
	state get_doing() const;
	//void check_surounding_tiles(Level* level);
};