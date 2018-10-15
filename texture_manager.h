#pragma once
#include "game.h"

class Texture_manager
{
public:
	static SDL_Texture* load_texture(SDL_Renderer*, const char*);
};

