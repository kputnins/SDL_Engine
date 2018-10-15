#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "game.h"
#include "settings.h"
#include "texture_manager.h"

Game::Game(){
	window = nullptr;
	renderer = nullptr;
	tex_player_01 = nullptr;

	player = nullptr;
	map = nullptr;
	camera = nullptr;

	init_sdl();
	load_data();
}

Game::~Game(){
	quit_sdl();
}

void Game::init_sdl() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::string sdl_error_string(SDL_GetError());
		std::cout << "SDL could not initialize! SDL_Error: " + sdl_error_string + "\n";
	}

	//Initializes SDL Image
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::string sdl_error_string(IMG_GetError());
		std::cout << "Failed to initialize SDL_image:" + sdl_error_string + "\n";
	}

	//Initializes SDL ttf
	if (TTF_Init() < 0) {
		std::cout << "Failed to initialize SDL_ttf!\n";
	}

	//Initializes SDL Mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		std::string sdl_error_string(Mix_GetError());
		std::cout << "Failed to initialize SDL_mixer!" + sdl_error_string + "\n";
	}

	//Create window
	window = SDL_CreateWindow(sett::TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sett::WIDTH, sett::HEIGHT, 0);
	if (window == NULL) {
		std::string sdl_error_string(SDL_GetError());
		std::cout << "Window could not be created! SDL_Error: " + sdl_error_string + "\n";
	}

	//Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::string sdl_error_string(SDL_GetError());
		std::cout << "Failed to create renderer!" + sdl_error_string + "\n";
	}
}

void Game::quit_sdl() {
	delete map;
	delete player;
	delete camera;

	SDL_DestroyTexture(tex_player_01);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	window = nullptr;
	renderer = nullptr;
	map = nullptr;
	player = nullptr;
	camera = nullptr;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::load_data() {
	tex_player_01 = Texture_manager::load_texture(this->renderer, "assets/img/player_01.png");
}

void Game::new_game() {
	// Creates map object (loads tmx map and populates variables)
	map = new Map(renderer, "assets/map/01.tmx");
	
	// Loops through object groups - creats and places game objects on map
	for (auto object_group : map->map_object_groups) {
		for (auto object : object_group) {
			if (object.getName() == "player") {
				auto pos = object.getPosition();
				player = new Player(tex_player_01, pos.x, pos.y, &sdl_event);
			}
		}
	}

	// Creates camera object
	camera = new Camera(map);
}

void Game::run() {
	// Variables for FPS corrections
	const int frame_time = 1000 / sett::FPS;		// Time a frame should take for frame rate calculation
	Uint32 frame_start = 0;					// Time passed for frame rate calculation
	int frame_length = 0;					// Frame length for frame rate calculation
	Uint32 fps_lasttime = SDL_GetTicks();	// Time passed for FPS calculation
	Uint32 fps_frames = 0;					// Frames passed for FPS calculation
	Uint32 fps_current = 0;					// Current FPS for FPS calculation

	is_playing = true;

	// Game-loop
	while (is_playing) {
		// Time at frame start
		frame_start = SDL_GetTicks();

		get_events();
		update();
		draw();

		// Calculates how long a frame has taken
		frame_length = SDL_GetTicks() - frame_start;

		// If frame was too fast - adds a delay
		if (frame_length < frame_time) {
			SDL_Delay(frame_time - frame_length);
		}

		// FPS calculation every half-second
		fps_frames++;
		if (fps_lasttime < SDL_GetTicks() - 500)
		{
			fps_lasttime = SDL_GetTicks();
			fps_current = fps_frames * 2;
			SDL_SetWindowTitle(window, (sett::TITLE + " FPS - " + std::to_string(fps_current)).c_str());
			fps_frames = 0;
		}
	}
}

void Game::get_events() {
	if (SDL_PollEvent(&sdl_event)) {
		switch (sdl_event.type)	{
		case SDL_QUIT:
			is_playing = false;
			break;

		case SDL_KEYDOWN:
			switch (sdl_event.key.keysym.sym) {
			case SDLK_ESCAPE:
				is_playing = false;
				break;
			case SDLK_g:
				is_grid = !is_grid;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

void Game::update() {
	// Updates ubjects
	player->update();

	// Updates
	camera->update(player);
}

void Game::draw() const {
	//Clears renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draws map
	camera->apply_rect(&map->d_r);
	map->draw_map();

	// Draws grid
	if (is_grid) {
		draw_grid();
	}
	
	// Draws player
	camera->apply(&player->dst_r);
	SDL_RenderCopy(renderer, player->texture, &player->src_r, &player->dst_r);

	// Presents renderer to window
	SDL_RenderPresent(renderer);
}

void Game::draw_grid() const {
	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	for (int i = 0; i < sett::WIDTH; i = i + sett::TILESIZE) {
		SDL_RenderDrawLine(renderer, i, 0, i, sett::HEIGHT);
	}
	for (int i = 0; i < sett::HEIGHT; i = i + sett::TILESIZE) {
		SDL_RenderDrawLine(renderer, 0, i, sett::WIDTH, i);
	}
}