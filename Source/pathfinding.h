#pragma once
#include "raylib.h"
#include "system.h"
#include <vector>
#include <deque>
class Level;
struct Tile;


struct Tile_node
{
	Tile_node* previous_node;
	Tile* tile;
	int g;
	int h;
	int f;
};

class Pathfinder
{
	Tile* start;
	Tile* end;
	Tile_node current;
	std::deque<Tile_node> open_list;
	std::deque<Tile_node> closed_list;
	std::deque<Tile_node> refined_path;

	int manhattan_distance(Vector2 pos_1, Vector2 pos_2);
	int distance(Vector2 pos_1, Vector2 pos_2);
	int g_cost(int current_index);
	int find_cheapest_tile();
	bool contains(const std::vector<Tile_node> list, const Tile_node article);
	void evaluate(Tile* tile, int current_index);
	void check_surrounding_tiles(Level* level, int tile_index);
	void clear();
public:

	//std::deque<Tile_node> get_path(const std::deque<Tile_node>& _grid, const Tile& _start, const Tile& _end);
	std::deque<Tile_node> get_path(Level* level, Tile* _start, Tile* _end);
	bool create();
	void draw(Vector2 tile_size);

};