#pragma once
#include "SDL.h"
#include "Constants.h"
#include "Tile.h"
#include "TilePosition.h"

namespace pacman {
	enum TRAVEL_DIRECTION {
		TRAVEL_LEFT,
		TRAVEL_DOWN,
		TRAVEL_RIGHT,
		TRAVEL_UP,
		TRAVEL_DEFAULT
	};
	//the abstract base class for the player and ghosts
	class Character {
	public:
		/*
			public methods
		*/
		Character( int x, int y );
		SDL_Rect getRect() const;
		virtual void render() = 0;
		virtual void reset() = 0;
		virtual ~Character(){}
	protected:
		/*
			protected member variables
		*/
		//the bounding box for the character
		SDL_Rect mRect;
		//the x and y index of the tile that contains
		//the center point of the character
		TilePosition mPos;
		//current speed of travel
		int mCurrentSpeed;
		//current and next direction of travel
		TRAVEL_DIRECTION mCurrentDirection = TRAVEL_DEFAULT; //character is stopped
		TRAVEL_DIRECTION mNextDirection = TRAVEL_DEFAULT; //no next direction set yet

		/*
			protected methods
		*/
		virtual void die() = 0;
		void move(bool offLimitsAllowed = false);
		void reverseDirection();
		bool calculateTileIndices();
		SDL_bool checkCollision( bool offLimitsAllowed = false ) const;
		bool isAlignedWithTile() const;
		void getNewCoords( TRAVEL_DIRECTION dir, int &x, int &y );
		bool changeDirection( int &x, int &y, bool offLimitTilesAllowed = false );
		void alignWithTile( int &x, int speedX ) const;
		bool isMoveAlongSameLine( TRAVEL_DIRECTION dir1, TRAVEL_DIRECTION dir2 ) const;
		Tile * getNextTile( TRAVEL_DIRECTION dir ) const;
		Tile * getPreviousTile( TRAVEL_DIRECTION dir ) const;
	};
}