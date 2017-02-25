#pragma once
#include "Tile.h"
#include <unordered_set>

namespace pacman {
	class Board {
	public:
		/*
			public methods
		*/
		//singleton
		static Board & getInstance() {
			static Board instance;
			return instance;
		}
		Board( Board const& ) = delete;
		void operator=( Board const & ) = delete;

		//bool loadTiles( std::string pathToFile );
		bool loadTiles( int fileName );
		void freeTiles();
		Tile * getTile( int x, int y );
		Tile * getTile( TilePosition pos );
		int getPelletCount();
		void decrementPelletCount();
		void render();
		void reset();
		std::vector<Tile*> getTileNeighbors( Tile * tile );
		bool getClosestFreeTilePosition( int &x, int &y );
		bool outOfBounds( int x, int y );
		bool isTileDeadEnd( TilePosition pos );
		bool isTileDeadEnd( int x, int y );
	private:
		/*
			private methods
		*/
		//prevent instantiation
		Board() {}
		//clean up on destruction
		~Board() { freeTiles(); }

		/*
			private member variables
		*/
		//array to store the tiles
		Tile * mTiles[TILE_COLS][TILE_ROWS];
		//number of uneaten pellets
		int mPelletCount = 0;
	};
}