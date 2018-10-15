#include "settings.h"

namespace sett{
	// Window settings
	std::string TITLE = "FB ver 0.003";
	const int WIDTH = 1024;	// 16 * 64 or 32 * 32 or 64 * 16
	const int HEIGHT = 768;	// 16 * 48 or 32 * 24 or 64 * 12

	// Screen settings
	const int TILESIZE = 64;
	const int GRIDWIDTH = WIDTH / TILESIZE;
	const int GRIDHEIGHT = HEIGHT / TILESIZE;

	const int FPS = 60;

	// Player settings
	const int PLAYER_SPEED = 8;
}