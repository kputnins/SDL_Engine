#pragma once
#include "sprite.h"
#include "map.h"

class Camera
{
public:
	Camera(Map*);
	~Camera();
	void update(Player*);
	void apply(SDL_Rect*);
	void apply_rect(SDL_Rect*);
private:
	Map* map;
	SDL_Rect camera_rect;
};

