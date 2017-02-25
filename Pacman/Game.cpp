#include "Game.h"

namespace pacman {
	/*
		Public methods
	*/
	bool Game::initGame() {
		//success flag
		bool success = true;
		//get board instance
		mBoard = &Board::getInstance();
		//load the map
		//if ( mBoard != NULL &&  mBoard->loadTiles( DATA_DIR + MAP_FILE_NAME ) ) {
		if ( mBoard != NULL &&  mBoard->loadTiles( MAP_RESOURCE_NAME ) ) {
			//get scoreboard instance
			mScoreboard = &Scoreboard::getInstance();
			//create the player
			mPlayer = new Player( INITIAL_PLAYER_POS );
			//mark pellet at player's position as eaten, but don't count it towards the score
			mBoard->getTile( INITIAL_PLAYER_POS )->setPelletEaten( false );
			mBoard->decrementPelletCount();
			////create the enemies
			for ( int i = 0; i < GHOST_COUNT; i++ ) {
				mEnemies[i] = new Enemy( i );
			}
		}
		return success;
	}

	bool Game::initSDL() {
		//Initialization flag
		bool success = true;
		//Initialize SDL
		if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
			printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Create window
			mWindow = SDL_CreateWindow( "Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
			if ( mWindow == NULL ) {
				printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			} else {
				//Create renderer for window
				mRenderer = SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
				if ( mRenderer == NULL ) {
					printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
					success = false;
				} else {
					//initialize renderer color to black
					SDL_SetRenderDrawColor( mRenderer, 0x00, 0x00, 0x00, 0xFF );
				}
			}
		}
		return success;
	}

	//function that starts the main game loop
	void Game::run() {
		if ( !initSDL() || !initGame() ) {
			printf( "Unable to initialize the game!\n" );
		} else {
			//quit flag
			bool quit = false;
			//SDL event object to handle events with
			SDL_Event e;
			//while no quit command has been issued
			while ( !quit ) {
				//poll for events from queue
				while ( SDL_PollEvent( &e ) != 0 ) {
					//handle request
					if ( e.type == SDL_QUIT ) {
						//set the quit flag
						quit = true;
					} else if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F2 ) {
						//restart the game
						restart();
					} else if ( !mPlayer->isDying() && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p ) {
						//toggle pause when the P key is pressed but only while playing
						pause();
					} else if ( mRunning && e.type == SDL_KEYDOWN && !mGhostsInitialized ) {
						//initialize the ghosts on the first keypress
						for ( int i = 0; i < GHOST_COUNT;i++ ) {
							mEnemies[i]->init();
						}
						mGhostsInitialized = true;
					}
				}
				//clear the screen
				SDL_RenderClear( mRenderer );
				//if the game has not ended
				if ( mRunning && !mPaused ) {
					//let player handle keyboard events
					mPlayer->handleEvent( e );
					//move the player
					mPlayer->move();
					//move the enemies
					for ( int i = 0; i < GHOST_COUNT;i++ ) {
						mEnemies[i]->move();
					}
				}
				//render the level
				mBoard->render();
				//render the enemies
				for ( int i = GHOST_COUNT - 1; i >= 0;i-- ) {
					mEnemies[i]->render();
				}
				//render the player
				mPlayer->render();
				//render score and lives left
				mScoreboard->render();
				if ( mWon ) {
					//render the "You won!" text
					renderWinText();
				}
				if ( mLost ) {
					//render the "Game Over" text
					renderLoseText();
				}
				//update screen
				SDL_RenderPresent( mRenderer );
			}
			//close everything after the game has been closed
			close();
		}
	}

	void Game::pause() {
		mPaused = !mPaused;
	}

	void Game::restart() {
		//make sure game is running
		mRunning = true;
		//unpause if paused
		mPaused = false;
		//reset win state
		mWon = false;
		//reset lost state
		mLost = false;
		//reset the characters
		resetCharacters();
		//reset the board
		mBoard->reset();
		//reset score and lives
		mScoreboard->reset();
	}

	void Game::win() {
		//win
		mWon = true;
		//end game
		mRunning = false;
	}

	void Game::lose() {
		//lost
		mLost = true;
		//end game
		mRunning = false;
	}

	void Game::notifyPelletEaten( bool isEnergizer ) {
		//decrease remaining pellets
		mBoard->decrementPelletCount();
		//increase score
		int amount = isEnergizer ? ENERGIZER_POINTS : PELLET_POINTS;
		mScoreboard->increaseScore( amount );
		//if the pellet is an energizer, change enemy behaviour
		if ( isEnergizer ) {
			for ( int i = 0;i < GHOST_COUNT;i++ ) {
				if ( mEnemies[i] != NULL ) {
					mEnemies[i]->becomeVulnerable();
				}
			}
		}
		//check for win
		if ( mBoard->getPelletCount() == 0 ) {
			//all pellets have been eaten
			win();
		}
	}

	void Game::notifyPlayerMoved( TilePosition pos ) {
		//store player's current position for future queries
		mPlayerPosition = pos;
		//eat pellet at player's position
		getTile( pos )->setPelletEaten();
		//also notify ghosts
		for ( int i = 0; i < GHOST_COUNT; i++ ) {
			if ( mEnemies[i] != NULL ) {
				mEnemies[i]->setPlayerPosition( pos );
			}
		}
	}

	//a ghost is dead - get points
	void Game::notifyGhostKilled() {
		//increase score
		mScoreboard->increaseScore( GHOST_POINTS );
	}

	void Game::notifyPlayerKilled() {
		//prevent player from being killed twice
		//this could happen if two ghosts catch him at the same time
		if ( !mPlayer->isDying() ) {
			//pause game
			pause();
			//kill player
			mPlayer->die();
			//lose a life when player dies
			mScoreboard->loseLife();
		}
	}

	void Game::resetCharacters() {
		//reset player
		mPlayer->reset();
		//reset ghosts
		mGhostsInitialized = false;
		//reset ghosts
		for ( int i = 0;i < GHOST_COUNT;i++ ) {
			if ( mEnemies[i] != NULL ) {
				mEnemies[i]->reset();
			}
		}
	}

	TilePosition Game::getPlayerPosition() const {
		return mPlayerPosition;
	}

	Tile * Game::getTile( int x, int y ) const {
		if ( mBoard ) {
			return mBoard->getTile( x, y );
		}
		return NULL;
	}

	Tile * Game::getTile( TilePosition pos ) const {
		return this->getTile( pos.getX(), pos.getY() );
	}

	bool Game::isTileDeadEnd( TilePosition pos ) {
		return mBoard->isTileDeadEnd( pos );
	}

	bool Game::isTileDeadEnd( int x, int y ) {
		return mBoard->isTileDeadEnd( x, y );
	}

	bool Game::getClosestFreeTilePosition( int &x, int &y ) {
		return mBoard->getClosestFreeTilePosition( x, y );
	}

	/*
		Private methods
	*/
	void Game::renderWinText() const {
		//position text in the middle of the game field
		int xPosition = SCREEN_WIDTH / 2 - (4 * LETTER_WIDTH);
		int yPosition = (SCREEN_HEIGHT - SCOREBOARD_HEIGHT - LETTER_HEIGHT) / 2;
		//clip just the relevant text from the image file
		SDL_Rect clip = { 0, 3 * LETTER_HEIGHT, 8 * LETTER_WIDTH, LETTER_HEIGHT };
		TextureManager::getInstance().getTextsTexture()->render( xPosition, yPosition, &clip );
		renderRestartText( yPosition + LETTER_HEIGHT + TEXT_PADDING );
	}

	void Game::renderLoseText() const {
		//position text in the middle of the game field
		int xPosition = SCREEN_WIDTH / 2 - (9 * LETTER_WIDTH / 2);
		int yPosition = (SCREEN_HEIGHT - SCOREBOARD_HEIGHT - LETTER_HEIGHT) / 2;
		//clip just the relevant text from the image file
		SDL_Rect clip = { 0, 4 * LETTER_HEIGHT, 9 * LETTER_WIDTH, LETTER_HEIGHT };
		TextureManager::getInstance().getTextsTexture()->render( xPosition, yPosition, &clip );
		renderRestartText( yPosition + LETTER_HEIGHT + TEXT_PADDING );
	}

	void Game::renderRestartText( int yPos ) const {
		int xPos = SCREEN_WIDTH / 2 - (11 * LETTER_WIDTH / 2);
		//clip just the relevant text from the image file
		SDL_Rect clip = { 0, 5 * LETTER_HEIGHT, 11 * LETTER_WIDTH, LETTER_HEIGHT };
		TextureManager::getInstance().getTextsTexture()->render( xPos, yPos, &clip );
	}

	void Game::close() {
		//delete player
		if ( mPlayer != NULL ) {
			delete mPlayer;
			mPlayer = NULL;
		}
		//delete ghosts
		for ( int i = 0;i < GHOST_COUNT;i++ ) {
			if ( mEnemies[i] != NULL ) {
				delete mEnemies[i];
				mEnemies[i] = NULL;
			}
		}
		//destroy renderer	
		SDL_DestroyRenderer( mRenderer );
		mRenderer = NULL;
		//destroy window
		SDL_DestroyWindow( mWindow );
		mWindow = NULL;
		//quit SDL
		SDL_Quit();
	}
}