#include "pathfinding.h"
#include "level.h"

bool Pathfinder::create()
{
	start = nullptr;
	end = nullptr;
	


	return true;
}

//std::deque<Tile_node> Pathfinder::get_path(const std::deque<Tile_node>& _grid, const Tile& _start, const Tile& _end)
//{
//	clear();
//	Tile_node start_node;
//	start_node.tile = _start;
//	start_node.g = 0;
//	start_node.h = manhattan_distance(_start.position, _end.position);
//	start_node.f = start_node.g + start_node.h;
//	start_node.previous_node = nullptr;
//	open_list.push_back(start_node);
//
//	bool search_is_complete = false;
//	while (!search_is_complete && !open_list.empty())
//	{
//		int cheapest_tile = find_cheapest_tile();
//		current = open_list[cheapest_tile];
//
//		if (current.tile == end)
//		{
//			search_is_complete = true;
//		}
//		else
//		{
//			closed_list.push_back(current);
//			open_list.erase(open_list.begin() + cheapest_tile);
//			check_surrounding_tiles(level, closed_list.size() - 1);
//		}
//	}
//
//	return std::deque<Tile_node>();
//}

std::deque<Tile_node> Pathfinder::get_path(Level* level, Tile* _start, Tile* _end)
{
	open_list.clear();
	closed_list.clear();
	refined_path.clear();
	start = _start;
	end = _end;

	Tile_node start_node;
	start_node.tile = start;
	start_node.g = 0;
	start_node.h = manhattan_distance(start->position, end->position);
	start_node.f = start_node.g + start_node.h;
	start_node.previous_node = nullptr;
	open_list.push_back(start_node);

	bool search_is_complete = false;

	while (!search_is_complete && !open_list.empty())
	{
		int cheapest_tile = find_cheapest_tile();
		current = open_list[cheapest_tile];

		if (current.tile == end)
		{
			search_is_complete = true;
		}
		else
		{
			closed_list.push_back(current);
			open_list.erase(open_list.begin() + cheapest_tile);
			check_surrounding_tiles(level, closed_list.size() - 1);
		}
	}

	if (!search_is_complete)
	{

		return std::deque<Tile_node>();
	}

	if (current.tile == end)
	{
		while (current.previous_node)
		{
			refined_path.push_front(current);
			current = *(current.previous_node);
		}

		Tile_node start_node;
		start_node.tile = start;
		start_node.g = 0;
		start_node.h = manhattan_distance(start->position, end->position);
		start_node.f = start_node.g + start_node.h;
		start_node.previous_node = nullptr;

		refined_path.push_front(start_node);
	}

	return refined_path;
}

int Pathfinder::find_cheapest_tile()
{
	int cheapest_tile_index = 0;
	float cheapest_f = std::numeric_limits<float>::max();

	for (int i = 0; i < open_list.size(); i++)
	{
		float f_cost = open_list[i].f;
		if (f_cost < cheapest_f)
		{
			cheapest_f = f_cost;
			cheapest_tile_index = i;
		}
	}

	return cheapest_tile_index;
}

void Pathfinder::check_surrounding_tiles(Level* level, int tile_index)
{
	shared::Vector2i grid_dimension = level->get_grid_dimensions();
	shared::Vector2i current_index = level->get_2d_index(closed_list[tile_index].tile->position);

	// Check top-left tile
	shared::Vector2i top_left_tile = { current_index.x - 1, current_index.y - 1 };
	if (top_left_tile.y >= 0 && top_left_tile.x >= 0)
	{
		if (level->check_tile(top_left_tile))
		{
			evaluate(level->get_tile(top_left_tile), tile_index);
		}
	}

	// Check top tile
	shared::Vector2i tile_above = { current_index.x, current_index.y - 1 };
	if (tile_above.y >= 0)
	{
		if (level->check_tile(tile_above))
		{
			evaluate(level->get_tile(tile_above), tile_index);
		}
	}

	// Check top-right tile
	shared::Vector2i top_right_tile = { current_index.x + 1, current_index.y - 1 };
	if (top_right_tile.y >= 0 && top_right_tile.x < grid_dimension.x)
	{
		if (level->check_tile(top_right_tile))
		{
			evaluate(level->get_tile(top_right_tile), tile_index);
		}
	}

	// Check left tile
	shared::Vector2i left_tile = { current_index.x - 1, current_index.y };
	if (left_tile.x >= 0)
	{
		if (level->check_tile(left_tile))
		{
			evaluate(level->get_tile(left_tile), tile_index);
		}
	}

	// Check right tile
	shared::Vector2i right_tile = { current_index.x + 1, current_index.y };
	if (right_tile.x < grid_dimension.x)
	{
		if (level->check_tile(right_tile))
		{
			evaluate(level->get_tile(right_tile), tile_index);
		}
	}

	// Check bottom-left tile
	shared::Vector2i bottom_left_tile = { current_index.x - 1, current_index.y + 1 };
	if (bottom_left_tile.x >= 0 && bottom_left_tile.y < grid_dimension.y)
	{
		if (level->check_tile(bottom_left_tile))
		{
			evaluate(level->get_tile(bottom_left_tile), tile_index);
		}
	}

	// Check bottom tile
	shared::Vector2i tile_below = { current_index.x, current_index.y + 1 };
	if (tile_below.y < grid_dimension.y)
	{
		if (level->check_tile(tile_below))
		{
			evaluate(level->get_tile(tile_below), tile_index);
		}
	}

	// Check bottom-right tile
	shared::Vector2i bottom_right_tile = { current_index.x + 1, current_index.y + 1 };
	if (bottom_right_tile.x < grid_dimension.x && bottom_right_tile.y < grid_dimension.y)
	{
		if (level->check_tile(bottom_right_tile))
		{
			evaluate(level->get_tile(bottom_right_tile), tile_index);
		}
	}
}

void Pathfinder::clear()
{
	open_list.clear();
	closed_list.clear();
	refined_path.clear();

}


void Pathfinder::evaluate(Tile* tile, int current_index)
{
	for (int i = 0; i < closed_list.size(); i++)
	{
		if (tile == closed_list[i].tile)
		{
			return;
		}
	}

	int already_in_open_list = false;
	for (int i = 0; i < open_list.size(); i++)
	{
		if (tile == open_list[i].tile)
		{
			already_in_open_list = i;
		}
	}

	if (!already_in_open_list)
	{
		Tile_node new_tile_node;
		new_tile_node.previous_node = std::addressof(closed_list[current_index]);
		new_tile_node.tile = tile;
		new_tile_node.g = closed_list[current_index].g + g_cost(current_index);
		new_tile_node.h = manhattan_distance(tile->position, end->position);
		new_tile_node.f = new_tile_node.g + new_tile_node.h;
		open_list.push_back(new_tile_node);
	}
	else 
	{
		int alternative_f = open_list[already_in_open_list].h + closed_list[current_index].g + g_cost(current_index);
		if (alternative_f < open_list[already_in_open_list].f)
		{
			open_list[already_in_open_list].previous_node = std::addressof(closed_list[current_index]);
			open_list[already_in_open_list].g = closed_list[current_index].g + g_cost(current_index);
			open_list[already_in_open_list].f = open_list[already_in_open_list].g + open_list[already_in_open_list].h;

		}
	}
}


int Pathfinder::manhattan_distance(Vector2 pos_1, Vector2 pos_2)
{
	int width_distance = abs(pos_2.x - pos_1.x);
	int height_distance = abs(pos_2.y - pos_1.y);
	int manhattan = width_distance + height_distance;

	return manhattan;
}

int Pathfinder::distance(Vector2 pos_1, Vector2 pos_2)
{
	int distance = sqrt(pow(pos_2.x - pos_1.x, 2) + pow(pos_2.x - pos_1.x, 2));

	return distance;
}

int Pathfinder::g_cost(int current_index)
{
	int g_cost = 0;

	if (closed_list[current_index].previous_node)
	{
		g_cost = 10;

		if (closed_list[current_index].tile->position.x != closed_list[current_index].previous_node->tile->position.x &&
			closed_list[current_index].tile->position.y != closed_list[current_index].previous_node->tile->position.y)
		{
			g_cost = 14;
		}
	}

	return g_cost;
}


bool Pathfinder::contains(const std::vector<Tile_node> list, const Tile_node article)
{
	bool contains = false;
	for (int i = 0; i < list.size(); i++)
	{
		if (article.tile == list[i].tile)
		{
			contains = true;
		}
	}
	return contains;
}

void Pathfinder::draw(Vector2 tile_size)
{
	//for (Tile_node n : open_list)
	//{
	//	DrawRectangle(n.tile->position.x, n.tile->position.y, tile_size.x, tile_size.y, GREEN);
	//}

	//for (Tile_node n : closed_list)
	//{
	//	DrawRectangle(n.tile->position.x, n.tile->position.y, tile_size.x, tile_size.y, RED);
	//	
	//}
	//for (int i = 0; i < closed_list.size(); i++)
	//{
	//	std::string s = std::to_string(i);
	//	char const* pchar = s.c_str();
	//	DrawRectangle(closed_list[i].tile->position.x, closed_list[i].tile->position.y, tile_size.x, tile_size.y, RED);
	//	//DrawText(pchar, closed_list[i].tile->position.x, closed_list[i].tile->position.y, 10, WHITE);

	//}

	for (int i = 0; i < refined_path.size(); i++)
	{
		std::string s = std::to_string(i);
		char const* pchar = s.c_str();
		//DrawRectangle(refined_path[i].tile->position.x, refined_path[i].tile->position.y, tile_size.x, tile_size.y, PURPLE);
		//DrawText(pchar, refined_path[i].tile->position.x, refined_path[i].tile->position.y, 10, WHITE);
		DrawText(pchar, refined_path[i].tile->position.x, refined_path[i].tile->position.y, 10, RED);

	}

	//DrawRectangle(current.tile->position.x, current.tile->position.y, tile_size.x, tile_size.y, BLUE);

}