#pragma once

#include <SDL.h>

class Player
{
public:
	Player(SDL_Texture*, int, int, SDL_Event*);
	~Player();
	void update();
private:
	void get_keys();
public:
	SDL_Texture* texture;
	SDL_Rect src_r;
	SDL_Rect dst_r;
private:
	SDL_Event* sdl_event;
	int pos[2];
	int vel[2] = { 0, 0 };
	int speed;
};

