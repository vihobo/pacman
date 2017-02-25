#include "Tile.h"
#include "TextureManager.h"
#include "Game.h"

namespace pacman {

	/*
		public methods
	*/
	Tile::Tile( int x, int y, TileType type ) {
		//set tile position in the map array
		mPos.setX( x / TILE_SIZE );
		mPos.setY( y / TILE_SIZE );
		//upper left corner of tile
		mRect.x = x;
		mRect.y = y;
		//set dimensions of the tile
		mRect.w = TILE_SIZE;
		mRect.h = TILE_SIZE;
		//set the type of the tile
		mType = type;
		if ( mType == TILE_PELLET || mType == TILE_ENERGIZER ) {
			//mark the tile as still having a pellet that has not been eaten yet
			mPelletEaten = false;
		}
	}

	void Tile::render() {
		if ( mType == TILE_WALL ) {
			//render the wall tile
			SDL_Rect clip = { 0, 0, mRect.w, mRect.h };
			TextureManager::getInstance().getTileTexture()->render( mRect.x, mRect.y, &clip );
		}
		if ( !mPelletEaten ) {
			if ( mType == TILE_PELLET ) {
				//render the pellet if it hasn't been eaten yet
				SDL_Rect clip = { TILE_SIZE, 0, mRect.w, mRect.h };
				TextureManager::getInstance().getTileTexture()->render( mRect.x, mRect.y, &clip );
			}
			if ( mType == TILE_ENERGIZER ) {
				//render the energizer if it hasn't been eaten yet
				int offset = (mFrameCounter / 15 == 0) ? 2 : 3;
				SDL_Rect clip = { offset * TILE_SIZE, 0, mRect.w, mRect.h };
				TextureManager::getInstance().getTileTexture()->render( mRect.x, mRect.y, &clip );
				mFrameCounter = (mFrameCounter + 1) % 30;
			}
		}
	}

	//reset tile and return true if a pellet is restored and false otherwise
	bool Tile::reset() {
		bool oldEatenState = mPelletEaten;
		//reset eaten pellets - no pellet on player's initial position
		mPelletEaten = ((mType == TILE_PELLET || mType == TILE_ENERGIZER) && mPos != INITIAL_PLAYER_POS) ? false : true;
		//reset frame counter
		mFrameCounter = 0;
		if ( oldEatenState != mPelletEaten ) {
			return true;
		}
		return false;
	}

	TileType Tile::getType() const {
		return mType;
	}

	SDL_Rect Tile::getRect() const {
		return mRect;
	}

	void Tile::setPelletEaten( bool notify /*=true*/ ) {
		//check if pellet has been eaten already
		if ( !mPelletEaten ) {
			//mark pellet as eaten
			mPelletEaten = true;
			if ( notify ) {
				//flag whether the pellet was an energizer
				bool isEnergizer = mType == TILE_ENERGIZER ? true : false;
				//notify the game about it
				Game::getInstance().notifyPelletEaten( isEnergizer );
			}
		}
	}

	bool Tile::isFloorTile( bool offLimitsAllowed /*=false*/ ) const {
		return mType == TILE_PELLET || mType == TILE_ENERGIZER || (offLimitsAllowed && mType == TILE_OFF_LIMITS);
	}

	TilePosition Tile::getPosition() const {
		return mPos;
	}
}