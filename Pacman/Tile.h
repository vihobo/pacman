#pragma once
#include <SDL.h>
#include "Constants.h"
#include "WTexture.h"
#include "TilePosition.h"

namespace pacman {
	enum TileType {
		TILE_PELLET,
		TILE_WALL,
		TILE_ENERGIZER,
		TILE_OFF_LIMITS,
		TILE_TYPES_NUM
	};
	class Tile {
	public:
		/*
			public methods
		*/
		Tile(int x, int y, TileType type);
		void render();
		bool reset();
		TileType getType() const;
		SDL_Rect getRect() const;
		void setPelletEaten(bool notify = true);
		bool isFloorTile(bool offLimitsAllowed = false) const;
		TilePosition getPosition() const;
	private:
		/*
			private member variables
		*/
		TilePosition mPos;
		TileType mType;
		SDL_Rect mRect;
		bool mPelletEaten = true;
		int mFrameCounter = 0;
	};
}