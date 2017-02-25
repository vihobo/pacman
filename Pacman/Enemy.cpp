#include "Enemy.h"
#include "TextureManager.h"
#include "Game.h"
#include "BFS.h"
#include "PathSearchNode.h"

namespace pacman {
	/*
		public methods
	*/
	Enemy::Enemy( int index ) : Character( INITIAL_GHOST_POS.getX() * TILE_SIZE + (index + 1) * GHOST_SPAWN_OFFSET, INITIAL_GHOST_POS.getY() * TILE_SIZE ) {
		//set ghost index
		mId = index;
		//set ghost scatter target index
		mScatterId = index;
	}

	void Enemy::init() {
		//reset speed
		mCurrentSpeed = CHARACTER_SPEED;
		//make sure ghost is alive
		mIsDead = false;
		//enemies start moving left at the beginning
		mCurrentDirection = TRAVEL_LEFT;
		mNextDirection = TRAVEL_DEFAULT;
		//set initial target
		mCurrentTarget = INITIAL_GHOST_TARGET;
		//set initial state
		mState = ENEMY_INITIAL_STATE;
		//start moving the ghost
		mIsMoving = true;
		//no target in this phase
		mMoveToTarget = false;
		//initialize the start phase timer
		mPhaseStart = SDL_GetTicks();
	}

	void Enemy::render() {
		//set clipping rectangle
		SDL_Rect clip = { 0, (mId % GHOST_COUNT) * TILE_SIZE, mRect.w, mRect.h };
		if ( mState == ENEMY_VULNERABLE_STATE ) {
			//ghost should be rendered purple while vulnerable
			clip.y = GHOST_COUNT * TILE_SIZE;
		} else if ( mIsDead ) {
			//only the eyes of the ghost are rendered when dead
			clip.y = (GHOST_COUNT + 1) * TILE_SIZE;
		}
		//flip ghost texture every 20 frames
		SDL_RendererFlip flip = (mFlipCount / 30 == 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
		TextureManager::getInstance().getEnemiesTexture()->render( mRect.x, mRect.y, &clip, 0.0, NULL, flip );
		mFlipCount = (mFlipCount + 1) % 60;
	}

	void Enemy::move() {
		if ( mIsMoving ) {
			//change state if necessary
			calculateChangeState();
			//calculate new direction of travel
			calculateNewDirection();
			//move the character in the chosen direction
			Character::move( isInInitialPhase() || mIsDead );
			if ( calculateTileIndices() ) {
				//check if there is a collision with the player
				collideWithPlayer();
			}
		}
	}

	void Enemy::collideWithPlayer() {
		if ( mPos == Game::getInstance().getPlayerPosition() ) {
			if ( mState == ENEMY_CHASE_STATE || mState == ENEMY_SCATTER_STATE ) {
				//kill player
				Game::getInstance().notifyPlayerKilled();
			} else if ( mState == ENEMY_VULNERABLE_STATE ) {
				//kill ghost
				die();
			}
		}
	}

	void Enemy::setPlayerPosition( TilePosition pos ) {
		//the player's position is the target during a chase
		if ( mState == ENEMY_CHASE_STATE ) {
			mCurrentTarget = pos;
		}
		//player has moved so check if he hit a ghost
		collideWithPlayer();
	}

	void Enemy::becomeVulnerable() {
		//can not become vulnerable while in the pen or dead
		if ( !isInInitialPhase() && !mIsDead ) {
			//switch state
			mState = ENEMY_VULNERABLE_STATE;
			//target is the same as in scatter mode
			//mCurrentTarget = getScatterTarget();
			getFreeScatterTarget( mId, mCurrentTarget );
			//reverse direction of travel
			reverseDirection();
			//decrease speed
			mCurrentSpeed = GHOST_VULNERABLE_SPEED;
			//start a timer for this phase
			mPhaseStart = SDL_GetTicks();
		}
	}

	void Enemy::reset() {
		//initialize with starting parameters
		init();
		//set initial position
		mRect.x = INITIAL_GHOST_POS.getX() * TILE_SIZE + (mId + 1) * GHOST_SPAWN_OFFSET;
		mRect.y = INITIAL_GHOST_POS.getY() * TILE_SIZE;
		//recalculate tile position
		calculateTileIndices();
		//stop movement
		mIsMoving = false;
	}

	/*
		private methods
	*/
	//kill the ghost
	void Enemy::die() {
		//set ghost dead
		mIsDead = true;
		//increase speed for going home
		mCurrentSpeed = GHOST_DEAD_SPEED;
		//set new target
		mCurrentTarget = INITIAL_GHOST_TARGET;
		//switch state
		mState = ENEMY_DEAD_STATE;
		//notify the game object
		Game::getInstance().notifyGhostKilled();
	}

	void Enemy::calculateChangeState() {
		if ( isPhaseOver() ) {
			//check if the initial phase is over
			if ( isInInitialPhase() && !mMoveToTarget ) {
				//make the ghost move to target
				mMoveToTarget = true;
			} else if ( mState == ENEMY_SCATTER_STATE ) {
				chase();
			} else if ( (mPos == mCurrentTarget && isAlignedWithTile()) || mState == ENEMY_CHASE_STATE || mState == ENEMY_VULNERABLE_STATE ) {
				//ghost is out of the pen, a chase or vulnerable phase has ended - set phase to scatter
				scatter();
			}
		} else if ( mIsDead && mPos == mCurrentTarget ) {
			if ( mCurrentTarget == INITIAL_GHOST_TARGET ) {
				//dead ghost is nearly home
				mCurrentTarget = INITIAL_GHOST_POS;
			} else {
				//ghost is inside the ghost house - reset to initial state
				init();
			}
		}
	}

	bool Enemy::isPhaseOver() const {
		switch ( mState ) {
			case ENEMY_INITIAL_STATE:
				return SDL_GetTicks() - mPhaseStart >= GHOST_INITIAL_PHASE_TIME * 1000;
				break;
			case ENEMY_SCATTER_STATE:
				return SDL_GetTicks() - mPhaseStart >= GHOST_SCATTER_PHASE_TIME * 1000;
				break;
			case ENEMY_CHASE_STATE:
				return SDL_GetTicks() - mPhaseStart >= GHOST_CHASE_PHASE_TIME * 1000;
				break;
			case ENEMY_VULNERABLE_STATE:
				return SDL_GetTicks() - mPhaseStart >= GHOST_VULNERABLE_PHASE_TIME * 1000;
				break;
			default:
				return false;
				break;
		}
	}

	void Enemy::scatter() {
		//reset scatter index
		mScatterId = mId;
		//make sure the ghost is not marked as dead
		mIsDead = false;
		//reset speed to normal
		mCurrentSpeed = CHARACTER_SPEED;
		//move towards target
		mMoveToTarget = true;
		//set new phase
		mState = ENEMY_SCATTER_STATE;
		//ghost is out of the pen - set new target and change state
		//this prevents him from entering pen again
		//mCurrentTarget = getScatterTarget();
		getFreeScatterTarget( mId, mCurrentTarget );
		//start a timer for this phase
		mPhaseStart = SDL_GetTicks();
	}

	void Enemy::chase() {
		//make sure the ghost is not marked as dead
		mIsDead = false;
		//reset speed to normal
		mCurrentSpeed = CHARACTER_SPEED;
		//follow target
		mMoveToTarget = true;
		//set new phase
		mState = ENEMY_CHASE_STATE;
		//get player's last position right away
		//this way even if the player is not moving, the ghosts can chase him
		mCurrentTarget = Game::getInstance().getPlayerPosition();
		//start phase time counter
		mPhaseStart = SDL_GetTicks();
	}

	//calculate direction using BFS
	void Enemy::calculateNewDirection() {
		//check if going towards target is allowed and ghost is aligned with the tile it is in
		if ( mMoveToTarget && isAlignedWithTile() ) {
			//target has been reached
			if ( mPos == mCurrentTarget ) {
				//choose next scatter target
				mScatterId++;
				getFreeScatterTarget( mScatterId, mCurrentTarget );
			}
			//perform a breadth first search to find the path to the goal
			BFS<PathSearchNode> bfs(TILE_COLS, TILE_ROWS);
			PathSearchNode start( Game::getInstance().getTile( mPos ) );
			PathSearchNode goal( Game::getInstance().getTile( mCurrentTarget ) );
			bfs.setStartAndGoalStates( start, goal );
			//if not standing still
			if ( mCurrentDirection != TRAVEL_DEFAULT ) {
				//get previous tile
				if ( Tile * prev = getPreviousTile( mCurrentDirection ) ) {
					//set the tile behind the ghost as visited
					//this will prevent the ghosts from turning around
					bfs.setVisited( prev->getPosition().getX(), prev->getPosition().getY() );
				}
			}
			bfs.search();
			if ( bfs.foundPath() ) {
				//get start of solution
				PathSearchNode * start = bfs.getSolutionStart();
				if ( start ) {
					//get the next node along the path towards the target
					PathSearchNode * next = bfs.getNextSolutionNode();
					if ( next ) {
						//figure out the new direction
						mNextDirection = next->getDirection( mPos );
					}
				}
			} else {
				//if the ghost is headed towards a dead end
				//it will not turn and it will hit a wall
				//try to find a way to get out of this
				resolveDeadEnds();
			}
			//free resources
			bfs.clear();
		}
	}

	////calculate direction using only local information
	//void Enemy::calculateNewDirection() {
	//	//check if going towards target is allowed and ghost is aligned with the tile it is in
	//	if ( mMoveToTarget && isAlignedWithTile() ) {
	//		//local temp variables
	//		int minTargetDistance = INT_MAX;
	//		TRAVEL_DIRECTION newDir = mCurrentDirection;

	//		//get next tile on the path
	//		Tile * nextTile = getNextTile( mCurrentDirection );
	//		//it could be NULL if the ghost is stationary
	//		if ( nextTile != NULL && nextTile->isFloorTile( isInInitialPhase() || mIsDead ) ) {
	//			minTargetDistance = nextTile->getPosition().getManhattanDistanceTo( mCurrentTarget );
	//		}

	//		//decide where the ghost should go to next - no turning around allowed
	//		if ( mCurrentDirection == TRAVEL_UP || mCurrentDirection == TRAVEL_DOWN ) {
	//			//choose either left or right if possible
	//			getShorterPath( TRAVEL_LEFT, newDir, minTargetDistance );
	//			getShorterPath( TRAVEL_RIGHT, newDir, minTargetDistance );
	//		}
	//		if ( mCurrentDirection == TRAVEL_LEFT || mCurrentDirection == TRAVEL_RIGHT ) {
	//			//choose either up or down if possible
	//			getShorterPath( TRAVEL_DOWN, newDir, minTargetDistance );
	//			getShorterPath( TRAVEL_UP, newDir, minTargetDistance );
	//		}
	//		mNextDirection = newDir;
	//		//if the ghost is headed towards a dead end
	//		//it will not turn and it will hit a wall
	//		//try to find a way to get out of this
	//		resolveDeadEnds();
	//	}
	//}

	void Enemy::resolveDeadEnds() {
		//deal with dead ends
		if ( Game::getInstance().isTileDeadEnd( mPos ) ) {
			//turn around if there is no other option
			reverseDirection();
			return;
		}
		Tile * tile = getNextTile( mCurrentDirection );
		if ( tile && tile->isFloorTile( true ) ) {
			//do nothing and let it move forward
		} else {
			if ( mCurrentDirection == TRAVEL_UP || mCurrentDirection == TRAVEL_DOWN ) {
				//choose either left or right if possible
				if ( canTravelInDirection( TRAVEL_LEFT ) ) { mCurrentDirection = TRAVEL_LEFT; return; }
				if ( canTravelInDirection( TRAVEL_RIGHT ) ) { mCurrentDirection = TRAVEL_RIGHT; return; }
			}
			if ( mCurrentDirection == TRAVEL_LEFT || mCurrentDirection == TRAVEL_RIGHT ) {
				//choose either up or down if possible
				if ( canTravelInDirection( TRAVEL_DOWN ) ) { mCurrentDirection = TRAVEL_DOWN; return; }
				if ( canTravelInDirection( TRAVEL_UP ) ) { mCurrentDirection = TRAVEL_UP; return; }
			}
		}
	}

	bool Enemy::canTravelInDirection( TRAVEL_DIRECTION dir ) {
		Tile * nextTile = getNextTile( dir );
		if ( nextTile != NULL &&  nextTile->isFloorTile( isInInitialPhase() || mIsDead ) ) {
			return true;
		}
		return false;
	}

	void Enemy::getShorterPath( TRAVEL_DIRECTION dirToCheck, TRAVEL_DIRECTION &bestDir, int &minDistance ) const {
		Tile * nextTile = getNextTile( dirToCheck );
		if ( nextTile != NULL && nextTile->isFloorTile( isInInitialPhase() || mIsDead ) ) {
			//calculate manhattan distance
			int distance = nextTile->getPosition().getManhattanDistanceTo( mCurrentTarget );
			//even if distance is equal, prefer to change direction
			if ( distance <= minDistance ) {
				//we have found a shorter path
				minDistance = distance;
				bestDir = dirToCheck;
			}
		}
	}

	TilePosition Enemy::getScatterTarget( int id ) const {
		//each ghost targets a corner of the map based on its ID as illustrated below
		/*
		0--2
		|  |
		1--3
		*/
		int x = (id >> 1) % 2;
		int y = id % 2;
		return TilePosition( x * (TILE_COLS - 1), y * (TILE_ROWS - 1) );
	}

	bool Enemy::getFreeScatterTarget( int id, TilePosition &result ) {
		static TilePosition * scatterTargets[GHOST_COUNT];
		//make sure the index is not too high
		id = id % GHOST_COUNT;
		if ( scatterTargets[id] != NULL ) {
			//return the already calculated tile position
			result = *scatterTargets[id];
			return true;
		}
		//calculate a target for the first time
		//get the initial scatter target for this ghost
		TilePosition target = getScatterTarget( id );
		//find the nearest empty cell to this one
		int x = target.getX();
		int y = target.getY();
		if ( Game::getInstance().getClosestFreeTilePosition( x, y ) ) {
			result.setX( x );
			result.setY( y );
			return true;
		}
		return false;
	}
}