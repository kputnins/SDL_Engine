#include <string>
#include <SDL_image.h>
#include "map.h"
#include "settings.h"

Map::Map(SDL_Renderer* ren, std::string file_path){
	renderer = ren;

	// Loads tmx map file
	map.load(file_path);

	// Loads map tilesets
	load_tilesets();

	create_layers();

	// Gets map size properties
	map_tile_count.x = map.getTileCount().x;
	map_tile_count.y = map.getTileCount().y;
	map_tile_size.x = map.getTileSize().x;
	map_tile_size.y = map.getTileSize().y;

	map_width = map_tile_count.x * map_tile_size.x;
	map_height = map_tile_count.y * map_tile_size.y;

	s_r.x = s_r.y = 0;
	d_r.x = d_r.y = 0;
	s_r.w = d_r.w = map_width;
	s_r.h = d_r.h = map_height;
}

Map::~Map(){
	printf("Map destroyed\n");
}

void Map::load_tilesets() {
	int tile_count = 0;

	// Loads tileset images in SDL_Surface vector
	std::vector<SDL_Surface*> map_tileset_surfaces;
	map_tileset_surfaces.reserve(map.getTilesets().size());
	for (int i = 0; i < map.getTilesets().size(); i++) {
		map_tileset_surfaces.emplace_back();
		map_tileset_surfaces[i] = IMG_Load(map.getTilesets()[i].getImagePath().c_str());
		tile_count += map.getTilesets()[i].getTileCount(); 
	}

	// Expands tileset tile clip vector to nesesary size
	tile_set_clips.reserve(tile_count);
	
	// Loops variables
	for (size_t n = 0; n < map_tileset_surfaces.size(); n++)	{
		int tile_count_x = map.getTilesets()[n].getColumnCount();
		int tile_count_y = map.getTilesets()[n].getTileCount() / map.getTilesets()[n].getColumnCount();
		int ts_width = map_tileset_surfaces[n]->w;
		int ts_height = map_tileset_surfaces[n]->h;

		int first_GID = map.getTilesets()[n].getFirstGID() - 1;

		// Converts tileset surface to texture and frees surface
		SDL_Texture* tileset_texture = SDL_CreateTextureFromSurface(renderer, map_tileset_surfaces[n]);
		SDL_FreeSurface(map_tileset_surfaces[n]);

		for (int i = 0; i < tile_count_y; i++) {
			for (int j = 0; j < tile_count_x; j++) {
				// Adds object - SDL Texture - to end of clip vector
				tile_set_clips.emplace_back();
				tile_set_clips[first_GID + j + i * tile_count_x] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ts_width / tile_count_x, ts_height / tile_count_y);

				// Renders a clip from the tileset to newly created texture in clip vector
				SDL_SetTextureBlendMode(tile_set_clips[first_GID + j + i * tile_count_x], SDL_BLENDMODE_BLEND);
				SDL_Rect rect = { j * ts_width / tile_count_x, i * ts_height / tile_count_y, ts_width / tile_count_x, ts_height / tile_count_y };
				SDL_SetRenderTarget(renderer, tile_set_clips[first_GID + j + i * tile_count_x]);
				SDL_RenderCopy(renderer, tileset_texture, &rect, NULL);
			}
		}
		// Destrojs tileset texture
		SDL_DestroyTexture(tileset_texture);
	}

	// Resets SDL render target
	SDL_SetRenderTarget(renderer, NULL);
}

void Map::create_layers() {
	// Gets layers from map file
	const auto& layers = map.getLayers();

	int it = 0;
	int io = 0;

	// Loops through layers and gets layer counts
	for (int i = 0; i < layers.size(); i++) {
		// Tile layers
		if (layers[i]->getType() == tmx::Layer::Type::Tile) {
			it++;
		}

		else if (layers[i]->getType() == tmx::Layer::Type::Object) {
			io++;
		}
	}

	// Expands layer vectors to nesesary size
	map_tile_layers.reserve(it);
	map_object_groups.reserve(io);

	it = 0;
	io = 0;

	// Loops through layers
	for (int i = 0; i < layers.size(); i++) {
		// Tile layers
		if (layers[i]->getType() == tmx::Layer::Type::Tile) {
			// Adds object - tmx::TileLayer - to end of tile layer vector
			map_tile_layers.emplace_back(NULL);
			const auto& layer = *dynamic_cast<const tmx::TileLayer*>(layers[i].get());
			map_tile_layers[it] = layer;
			it++;
		}

		// Objecy layers
		else if (layers[i]->getType() == tmx::Layer::Type::Object) {
			// Adds object - vector<tmx::TileLayer> - to end of object vector vector
			map_object_groups.emplace_back(NULL);
			const auto& objects = dynamic_cast<tmx::ObjectGroup*>(layers[i].get())->getObjects();
			map_object_groups[io] = objects;
			io++;
		}
	}
}

void Map::draw_map(){
	// Position of tile
	SDL_Rect sr, dr;
	sr.x = sr.y = 0;
	sr.w = dr.w = map_tile_size.x;
	sr.h = dr.h = map_tile_size.y;

	// Loops through layers
	for (auto layer : map_tile_layers) {
		// Gets tiles from layer
		auto tiles = layer.getTiles();

		// Loops through tiles in layer
		for (size_t i = 0; i < tiles.size(); i++){
			// If layer has a tile
			if (tiles[i].ID != 0) {
				// Sets the position of the tile
				dr.x = d_r.x + i % map_tile_count.x * map_tile_size.x;
				dr.y = d_r.y + (i / map_tile_count.x) * map_tile_size.y;

				// Checks if tile posiion is inside the screen
				if (dr.x > -sett::TILESIZE && dr.y > -sett::TILESIZE && dr.x < sett::WIDTH && dr.y < sett::HEIGHT) {
					// Draws tile as tile clip texture from tile clip texture vector
					SDL_RenderCopy(renderer, tile_set_clips[tiles[i].ID - 1], &sr, &dr);
				}
			}
		}
		// Resets the tile position for the next layer
		dr.x = 0;
		dr.y = 0;
	}
}