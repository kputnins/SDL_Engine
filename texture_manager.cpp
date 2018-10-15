#include <iostream>
#include <SDL_image.h>
#include "texture_manager.h"

SDL_Texture* Texture_manager::load_texture(SDL_Renderer* renderer, const char* file_name) {
	//Creates a surface
	SDL_Surface* temp_surface = IMG_Load(file_name);
	if (temp_surface == nullptr) {
		std::cout << "Failed to create temprorary surface!\n";
	}

	//Creates a texture from surface
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, temp_surface);
	if (!tex) {
		std::cout << "Failed to create texture!\n";
	}

	//After texture created, destroys temp surface
	SDL_FreeSurface(temp_surface);

	return tex;
}