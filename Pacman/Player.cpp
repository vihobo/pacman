#include "Player.h"
#include "TextureManager.h"
#include "Game.h"

namespace pacman {
	Player::Player( int x, int y ) : Character( x, y ) {

	}

	Player::Player( TilePosition pos ) : Player( pos.getX() * TILE_SIZE, pos.getY() * TILE_SIZE ) {

	}

	void Player::handleEvent( SDL_Event& e ) {
		//If a key was pressed
		if ( e.type == SDL_KEYDOWN ) {
			switch ( e.key.keysym.sym ) {
				case SDLK_UP:
					mNextDirection = TRAVEL_UP;
					break;
				case SDLK_DOWN:
					mNextDirection = TRAVEL_DOWN;
					break;
				case SDLK_LEFT:
					mNextDirection = TRAVEL_LEFT;
					break;
				case SDLK_RIGHT:
					mNextDirection = TRAVEL_RIGHT;
					break;
				default:
					break;
			}
		}
	}

	void Player::move() {
		if ( mCurrentDirection != TRAVEL_DEFAULT || mNextDirection != TRAVEL_DEFAULT ) {
			//move the character
			Character::move();
			//calculate tile position of player
			if ( calculateTileIndices() ) {
				//player has changed tiles - notify the game object
				Game::getInstance().notifyPlayerMoved( mPos );
			}
		}
	}

	void Player::render() {
		//show first image at start
		SDL_Rect clip = { 0, 0, TILE_SIZE, TILE_SIZE };
		SDL_RendererFlip flip = mCurrentDirection == TRAVEL_RIGHT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		double angle = mCurrentDirection == TRAVEL_UP ? 90.0 : (mCurrentDirection == TRAVEL_DOWN ? -90.0 : 0.0);
		if ( mFrameCounter / 10 == 0 ) {
			clip.x = 0;
		} else if ( mFrameCounter / 20 == 0 ) {
			clip.x = TILE_SIZE;
		} else {
			clip.x = 2 * TILE_SIZE;
		}
		if ( mIsDying ) {
			//render dying animation
			if ( mFrameCounter / 20 == 0 ) {
				clip.x = 3 * TILE_SIZE;
			} else if ( mFrameCounter / 40 == 0 ) {
				clip.x = 4 * TILE_SIZE;
			} else {
				clip.x = 5 * TILE_SIZE;
			}
			if ( mFrameCounter == 59 ) {
				//after dying animation is over reset characters
				Game::getInstance().resetCharacters();
				//not dying anymore
				mIsDying = false;
				//unpause the game
				Game::getInstance().pause();
			}
			//longer animation when dying
			mFrameCounter = (mFrameCounter + 1) % 60;
		} else {
			mFrameCounter = (mFrameCounter + 1) % 30;
		}
		TextureManager::getInstance().getPacmanTexture()->render( mRect.x, mRect.y, &clip, angle, NULL, flip );
	}

	void Player::die() {
		//start death animation
		mIsDying = true;
		//reset frame counter
		mFrameCounter = 0;
	}

	void Player::reset() {
		//reset to initial position
		mRect.x = INITIAL_PLAYER_POS.getX() * TILE_SIZE;
		mRect.y = INITIAL_PLAYER_POS.getY() * TILE_SIZE;
		//set the indices of the tile the character is in
		calculateTileIndices();
		//at the beginning all characters have the same speed
		mCurrentSpeed = CHARACTER_SPEED;
		//stop player
		mCurrentDirection = TRAVEL_DEFAULT;
		mNextDirection = TRAVEL_DEFAULT;
	}

	bool Player::isDying() const {
		return mIsDying;
	}
}