#pragma once

#include <SDL.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Object.hpp>

class Map
{
public:
	Map(SDL_Renderer*, std::string);
	~Map();
	void draw_map();
	int get_map_width() { return map_width; };
	int get_map_height() { return map_height; };
private:
	void load_tilesets();
	void create_layers();
public:
	std::vector< std::vector<tmx::Object> > map_object_groups;
	SDL_Rect d_r;
private:
	SDL_Renderer* renderer;
	tmx::Map map;
	SDL_Rect s_r;
	int map_width;
	int map_height;
	tmx::Vector2i map_tile_count;
	tmx::Vector2i map_tile_size;
	std::vector<SDL_Texture*> tile_set_clips;
	std::vector<tmx::TileLayer> map_tile_layers;
};

