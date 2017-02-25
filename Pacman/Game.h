#pragma once
#include <SDL.h>
#include "Constants.h"
#include "TextureManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Board.h"
#include "Scoreboard.h"

namespace pacman {
	class Game {
	public:
		/*
			public methods
		*/
		//singleton
		static Game & getInstance() {
			static Game instance;
			return instance;
		}
		Game( Game const& ) = delete;
		void operator=( Game const & ) = delete;

		bool initGame();
		bool initSDL();
		void run();
		void pause();
		void restart();
		void win();
		void lose();
		//callback function for eating pellets
		void notifyPelletEaten( bool isEnergizer );
		//callback function for player movement
		void notifyPlayerMoved( TilePosition pos );
		//get notification that a ghost has died
		void notifyGhostKilled();
		//get notification that the player has died
		void notifyPlayerKilled();
		//reset everybody to their starting positions
		void resetCharacters();
		//get the last position of the player
		TilePosition getPlayerPosition() const;
		Tile * getTile( int x, int y ) const;
		Tile * getTile( TilePosition pos ) const;
		bool isTileDeadEnd( TilePosition pos );
		bool isTileDeadEnd( int x, int y );
		bool getClosestFreeTilePosition( int &x, int &y );
		inline SDL_Renderer * getRenderer() { return mRenderer; }
	private:
		/*
			private methods
		*/
		//prevent instantiation
		Game() {}
		void renderWinText() const;
		void renderLoseText() const;
		void renderRestartText( int yPos ) const;
		void close();
		~Game() { close(); }

		/*
			private member variables
		*/
		SDL_Window * mWindow = NULL;
		SDL_Renderer * mRenderer = NULL;
		Player * mPlayer = NULL;
		TilePosition mPlayerPosition = TilePosition( INITIAL_PLAYER_POS );
		Enemy * mEnemies[GHOST_COUNT];
		Board * mBoard = NULL;
		Scoreboard * mScoreboard = NULL;
		//flag indicating if the ghosts have been initialized
		bool mGhostsInitialized = false;
		//flag indicating if the game is running
		bool mRunning = true;
		//flag indicating if the game is paused
		bool mPaused = false;
		//flag indicating if the game was won
		bool mWon = false;
		//flag indicating if the game was lost
		bool mLost = false;
	};
}