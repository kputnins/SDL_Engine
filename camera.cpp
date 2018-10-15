#include <iostream>
#include <algorithm>
#include "camera.h"
#include "settings.h"

Camera::Camera(Map* m){
	map = m;
	camera_rect.x = camera_rect.y = 0;
	camera_rect.w = sett::WIDTH;
	camera_rect.h = sett::HEIGHT;
}

Camera::~Camera(){
	printf("Camera destroyed\n");
}

void Camera::update(Player* target) {
	int x =int(sett::WIDTH / 2) - (target->dst_r.x + target->dst_r.w / 2);
	int y =int(sett::HEIGHT / 2) - (target->dst_r.y + target->dst_r.h / 2);

	// Limit scrolling to map size
	x = std::min(0, x);											//Left
	y = std::min(0, y);											//Top
	x = std::max(-(map->get_map_width() - sett::WIDTH), x);		//Right
	y = std::max(-(map->get_map_height() - sett::HEIGHT), y);	//Bottom
	
	camera_rect.x = x;
	camera_rect.y = y;
}

void Camera::apply(SDL_Rect* rect) {
	rect->x += camera_rect.x;
	rect->y += camera_rect.y;
}

void Camera::apply_rect(SDL_Rect* rect) {
	rect->x = camera_rect.x;
	rect->y = camera_rect.y;
}