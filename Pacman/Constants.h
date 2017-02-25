#pragma once
#include <string>
#include "TilePosition.h"
#include "resource.h"

namespace pacman {
	//text constants
	static const int LETTER_WIDTH = 15;
	static const int LETTER_HEIGHT = 17;
	static const int TEXT_PADDING = 5;
	//tile constants
	static const int TILE_COLS = 19;
	static const int TILE_ROWS = 19;
	static const int TILE_SIZE = 32;
	//screen dimensions
	static const int SCOREBOARD_HEIGHT = 2 * TEXT_PADDING + LETTER_HEIGHT;
	static const int SCREEN_WIDTH = TILE_COLS * TILE_SIZE;
	static const int SCREEN_HEIGHT = TILE_ROWS * TILE_SIZE + SCOREBOARD_HEIGHT;
	//number of ghosts and of player's lives
	static const int GHOST_COUNT = 4;
	static const int PLAYER_LIFE_COUNT = 3;
	//default character speed
	static const int CHARACTER_SPEED = 4;
	static const int GHOST_VULNERABLE_SPEED = 2;
	static const int GHOST_DEAD_SPEED = 8;
	static const int PELLET_POINTS = 20;
	static const int ENERGIZER_POINTS = 40;
	static const int GHOST_POINTS = 100;
	////dirs
	//static const std::string DATA_DIR = "Resources/";
	////static const std::string DATA_DIR = "";
	//static const std::string IMAGE_SUB_DIR = "bitmaps/";
	////static const std::string IMAGE_SUB_DIR = "";
	////map file name
	//static const std::string MAP_FILE_NAME = "map.txt";
	////bitmap names
	//static const std::string TILE_FILE_NAME = "tile.bmp";
	//static const std::string PACMAN_FILE_NAME = "pacman.bmp";
	//static const std::string ENEMIES_FILE_NAME = "ghosts.bmp";
	//static const std::string TEXTS_FILE_NAME = "texts.bmp";
	//resource names
	static const int MAP_RESOURCE_NAME = IDR_MAPFILE;
	static const int TILE_BITMAP_RESOURCE_NAME = IDB_TILE_BITMAP;
	static const int PACMAN_BITMAP_RESOURCE_NAME = IDB_PACMAN_BITMAP;
	static const int GHOSTS_BITMAP_RESOURCE_NAME = IDB_GHOSTS_BITMAP;
	static const int TEXTS_BITMAP_RESOURCE_NAME = IDB_TEXTS_BITMAP;
	//resource type names
	static const int TEXTFILE_RESOURCE_TYPE = TEXTFILE;
	//initial player position
	static const TilePosition INITIAL_PLAYER_POS( 9, 12 );
	//static const TilePosition INITIAL_PLAYER_POS( 0, 0 );
	//initial ghost position as tile indices on the map
	static const TilePosition INITIAL_GHOST_POS( 8, 8 );
	//the ghost's target for the end of the initial phase
	//that lets them get out of their pen
	static const TilePosition INITIAL_GHOST_TARGET( 9, 6 );
	//each ghost is offset by index * offset from the initial ghost position
	//so that they are not spawned on top of each other
	static const int GHOST_SPAWN_OFFSET = 10;
	//time for the ghosts' initial phase in seconds
	static const int GHOST_INITIAL_PHASE_TIME = 5;
	//time for scattering
	static const int GHOST_SCATTER_PHASE_TIME = 4;
	//time for chasing
	static const int GHOST_CHASE_PHASE_TIME = 10;
	//time for running
	static const int GHOST_VULNERABLE_PHASE_TIME = 8;
}