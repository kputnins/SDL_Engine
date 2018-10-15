#pragma once

#include <SDL.h>
#include "sprite.h"
#include "map.h"
#include "camera.h"

class Map;

class Game
{
public:
	Game();
	~Game();
	void new_game();
	void run();
private:
	void init_sdl();
	void quit_sdl();
	void load_data();
	void get_events();
	void update();
	void draw() const;
	void draw_grid() const;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* tex_player_01;
	SDL_Event sdl_event;
	Player* player;
	Map* map;
	Camera* camera;
	bool is_playing;
	bool is_grid = false;
	int map_widht;
	int map_height;
};