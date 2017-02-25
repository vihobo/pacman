#include "Character.h"
#include "Game.h"

namespace pacman {
	/*
		public methods
	*/
	Character::Character( int x, int y ) {
		//upper left corner of character
		mRect.x = x;
		mRect.y = y;
		//set dimensions of the character
		mRect.w = TILE_SIZE;
		mRect.h = TILE_SIZE;
		//set the indices of the tile the character is in
		calculateTileIndices();
		//at the beginning all characters have the same speed
		mCurrentSpeed = CHARACTER_SPEED;
	}

	SDL_Rect Character::getRect() const {
		return mRect;
	}

	/*
		protected methods
	*/
	//move character on the board
	void Character::move( bool offLimitsAllowed/* = false*/ ) {
		//placeholders for new x and y position
		int newX, newY;
		//try to change direction
		bool newPosSet = changeDirection( newX, newY, offLimitsAllowed );
		//if that did not happen, continue using the current direction of travel
		if ( !newPosSet ) {
			//calculate x and y after move
			getNewCoords( mCurrentDirection, newX, newY );
		}
		//store old position for now
		int oldX = mRect.x;
		int oldY = mRect.y;
		//actually move the character to the newly calculated position
		mRect.x = newX;
		mRect.y = newY;
		//check if it is possible to move
		//using the off limit tiles is allowed in the initial ghost phase
		if ( checkCollision( offLimitsAllowed ) == SDL_TRUE ) {
			//can not move in this direction
			//revert to old position
			mRect.x = oldX;
			mRect.y = oldY;
			//if the ghost is in its initial phase, it goes in circles
			if ( offLimitsAllowed ) {
				//get next direction for the circular movement
				mCurrentDirection = static_cast< TRAVEL_DIRECTION >((mCurrentDirection + 1) % TRAVEL_DEFAULT);
			}
		}
	}

	void Character::reverseDirection() {
		switch ( mCurrentDirection ) {
			case TRAVEL_DOWN:
				mCurrentDirection = TRAVEL_UP;
				break;
			case TRAVEL_UP:
				mCurrentDirection = TRAVEL_DOWN;
				break;
			case TRAVEL_LEFT:
				mCurrentDirection = TRAVEL_RIGHT;
				break;
			case TRAVEL_RIGHT:
				mCurrentDirection = TRAVEL_LEFT;
				break;
		}
		//make sure that the direction of travel will change
		mNextDirection = TRAVEL_DEFAULT;
	}

	//calculate the tile in which the character's center point lies
	//return true if the tile has changed
	bool Character::calculateTileIndices() {
		//make a copy of the current character position
		TilePosition oldPos( mPos );
		//recalculate the indices of the tile the player is in
		mPos.setX( (mRect.x + (TILE_SIZE / 2)) / TILE_SIZE );
		mPos.setY( (mRect.y + (TILE_SIZE / 2)) / TILE_SIZE );
		//check for change in position
		if ( oldPos != mPos ) {
			//player has changed tiles - notify the game object
			return true;
		}
		return false;
	}

	//check collision with the next tile on the character's path
	SDL_bool Character::checkCollision( bool offLimitsAllowed /*=false*/ ) const {
		Tile  * nextTile = getNextTile( mCurrentDirection );
		if ( nextTile != NULL && !nextTile->isFloorTile( offLimitsAllowed ) ) {
			return SDL_HasIntersection( &mRect, &nextTile->getRect() );
		}
		return SDL_FALSE;
	}

	//check if the character occupies exactly a single tile
	bool Character::isAlignedWithTile() const {
		return mRect.x % TILE_SIZE == 0 && mRect.y % TILE_SIZE == 0;
	}

	//get character coordinates when traveling in the current direction
	void Character::getNewCoords( TRAVEL_DIRECTION dir, int &x, int &y ) {
		//calculate new coordinates after a move
		int speedX = (dir == TRAVEL_LEFT) ? -mCurrentSpeed : ((dir == TRAVEL_RIGHT) ? mCurrentSpeed : 0);
		int speedY = (dir == TRAVEL_UP) ? -mCurrentSpeed : ((dir == TRAVEL_DOWN) ? mCurrentSpeed : 0);
		x = mRect.x;
		y = mRect.y;
		//if the character is actually moving
		if ( speedX != 0 || speedY != 0 ) {
			//move character in a way as to not skip tiles
			alignWithTile( x, speedX );
			alignWithTile( y, speedY );
		}
	}

	//try to switch the direction of travel to the newly chosen one
	bool Character::changeDirection( int &x, int &y, bool offLimitTilesAllowed/*=false*/ ) {
		//flag indicating whether a new position has been calculated
		bool newPosSet = false;
		//placeholders for the new position
		x = mRect.x;
		y = mRect.y;
		//check if a change of direction has been requested
		if ( mNextDirection != TRAVEL_DEFAULT && mNextDirection != mCurrentDirection ) {
			//try to change direction
			if ( isMoveAlongSameLine( mNextDirection, mCurrentDirection ) || isAlignedWithTile() ) {
				if ( isAlignedWithTile() ) {
					//flag indicating whether it is safe to continue
					bool continueLogic = false;
					//if there is a tile in this direction
					if ( Tile * tile = getNextTile( mNextDirection ) ) {
						//and it is a floor tile
						if ( tile->isFloorTile( offLimitTilesAllowed ) ) {
							//then we can go on
							continueLogic = true;
						}
					}
					//if this failed, then we are trying to move out of bounds
					if ( !continueLogic ) {
						return false;
					}
				}
				//calculate x and y after move
				getNewCoords( mNextDirection, x, y );
				//switch current direction
				mCurrentDirection = mNextDirection;
				//reset next direction
				mNextDirection = TRAVEL_DEFAULT;
				//mark new position as set
				newPosSet = true;
			}
		}
		return newPosSet;
	}

	//prevent character from jumping over the beginning of any tile
	//thus missing opportunities to turn at intersections
	/*
	Note that this function effectively limits the character speed
	to at most TILE_SIZE pixels per frame
	*/
	void Character::alignWithTile( int &x, int speedX ) const {
		int remainderX = x % TILE_SIZE;
		if ( remainderX + speedX > TILE_SIZE ) {
			//jumped over the beginning of next tile - do not skip it
			x += (TILE_SIZE - remainderX);
		} else if ( remainderX > 0 && remainderX + speedX < 0 ) {
			//jumped back too much
			x -= remainderX;
		} else if ( remainderX + speedX < -TILE_SIZE ) {
			//jumped over one tile back - do not skip it
			x -= remainderX + TILE_SIZE;
		} else {
			x += speedX;
		}
	}

	//check whether these two directions lie on the same line
	bool Character::isMoveAlongSameLine( TRAVEL_DIRECTION dir1, TRAVEL_DIRECTION dir2 ) const {
		return ((dir1 == TRAVEL_LEFT || dir1 == TRAVEL_RIGHT) && (dir2 == TRAVEL_LEFT || dir2 == TRAVEL_RIGHT))
			|| ((dir1 == TRAVEL_UP || dir1 == TRAVEL_DOWN) && (dir2 == TRAVEL_UP || dir2 == TRAVEL_DOWN));
	}

	//get next tile in the direction of travel
	Tile * Character::getNextTile( TRAVEL_DIRECTION dir ) const {
		//no next tile if standing still
		if ( dir != TRAVEL_DEFAULT ) {
			int xIndex = mPos.getX() + (dir == TRAVEL_RIGHT ? 1 : (dir == TRAVEL_LEFT ? -1 : 0));
			int yIndex = mPos.getY() + (dir == TRAVEL_DOWN ? 1 : (dir == TRAVEL_UP ? -1 : 0));
			return Game::getInstance().getTile( xIndex, yIndex );
		}
		return NULL;
	}

	//get previous tile in the direction of travel
	Tile * Character::getPreviousTile( TRAVEL_DIRECTION dir ) const {
		//no next tile if standing still
		if ( dir != TRAVEL_DEFAULT ) {
			int xIndex = mPos.getX() + (dir == TRAVEL_RIGHT ? -1 : (dir == TRAVEL_LEFT ? 1 : 0));
			int yIndex = mPos.getY() + (dir == TRAVEL_DOWN ? -1 : (dir == TRAVEL_UP ? 1 : 0));
			return Game::getInstance().getTile( xIndex, yIndex );
		}
		return NULL;
	}

}