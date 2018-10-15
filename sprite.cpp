#include <iostream>
#include "sprite.h"
#include "settings.h"

Player::Player(SDL_Texture* t, int x, int y, SDL_Event* evnt){
	sdl_event = evnt;

	texture = t;
	pos[0] = x;
	pos[1] = y;

	src_r.x = 0;
	src_r.y = 0;
	src_r.w = 49;
	src_r.h = 49;

	dst_r.x = pos[0];
	dst_r.y = pos[1];
	dst_r.w = sett::TILESIZE;
	dst_r.h = sett::TILESIZE;

	speed = sett::PLAYER_SPEED;
}

Player::~Player(){
	printf("Sprite destroyed\n");
}

void Player::update() {
	get_keys();

	pos[0] += vel[0];
	pos[1] += vel[1];
	dst_r.x = pos[0];
	dst_r.y = pos[1];
}

void Player::get_keys() {
	vel[0] = 0;
	vel[1] = 0;

	if (sdl_event->type == SDL_KEYDOWN)
		switch (sdl_event->key.keysym.sym) {
		case SDLK_UP:
			vel[1] = -speed;
			break;
		case SDLK_DOWN:
			vel[1] = speed;
			break;
		case SDLK_LEFT:
			vel[0] = -speed;
			break;
		case SDLK_RIGHT:
			vel[0] = speed;
			break;
		case SDLK_w:
			vel[1] = -speed;
			break;
		case SDLK_s:
			vel[1] = speed;
			break;
		case SDLK_a:
			vel[0] = -speed;
			break;
		case SDLK_d:
			vel[0] = speed;
			break;
		case SDLK_SPACE:
			break;
		default:
			break;
		}
}