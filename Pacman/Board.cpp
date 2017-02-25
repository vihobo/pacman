#include "Board.h"
#include "ResourceLoader.h"
#include "BFS.h"
#include "FreeTileSearchNode.h"

namespace pacman {
	/*
		public methods
	*/
	//bool Board::loadTiles( std::string pathToFile ) {
	bool Board::loadTiles( int fileName ) {
		//success flag
		bool success = true;
		//open the map
		DWORD size = 0;
		const char* data = NULL;
		ResourceLoader::LoadFileInResource( IDR_MAPFILE, TEXTFILE_RESOURCE_TYPE, size, data );
		/* Access bytes in data - here's a simple example involving text output*/
		// The text stored in the resource might not be NULL terminated.
		char* buffer = new char[size + 1];
		memcpy( buffer, data, size );
		buffer[size] = 0; // NULL terminator
		membuf sbuf( buffer, buffer + size );
		std::istream map( &sbuf );

		//open the map
		//std::ifstream map( pathToFile );
		//check if the map is open
		//if ( !map.is_open() ) {
		if ( !map.good() ) {
			printf( "Unable to load map file!\n" );
			success = false;
		} else {
			//create the tiles
			int x = 0, y = 0;
			while ( !map.eof() ) {
				int tileType;
				map >> tileType;
				if ( map.fail() ) {
					printf( "Could not load map!\n" );
					success = false;
					break;
				}
				if ( tileType >= 0 && tileType < TILE_TYPES_NUM ) {
					//type is ok, create tile
					mTiles[x][y] = new Tile( x * TILE_SIZE, y * TILE_SIZE, static_cast< TileType >(tileType) );
					if ( tileType == TILE_PELLET || tileType == TILE_ENERGIZER ) {
						mPelletCount++;
					}
				} else {
					//unknown tile type in map file
					printf( "Unable to load unknown tile type from map file!\n" );
					success = false;
					break;
				}
				//increment x
				x++;
				//if we are at the end of the row, go to next one
				if ( x >= TILE_COLS ) {
					//move to the start of line
					x = 0;
					//move one row down
					y++;
				}
			}
		}
		return success;
	}

	void Board::freeTiles() {
		//deallocate tiles
		for ( int y = 0; y < TILE_ROWS; ++y ) {
			for ( int x = 0; x < TILE_COLS; ++x ) {
				if ( mTiles[x][y] != NULL ) {
					delete mTiles[x][y];
					mTiles[x][y] = NULL;
				}
			}
		}
	}

	Tile * Board::getTile( int x, int y ) {
		if ( !outOfBounds( x, y ) ) {
			return mTiles[x][y];
		}
		//x and y out of bounds
		return NULL;
	}

	Tile * Board::getTile( TilePosition pos ) {
		return this->getTile( pos.getX(), pos.getY() );
	}

	int Board::getPelletCount() {
		return mPelletCount;
	}

	void Board::decrementPelletCount() {
		mPelletCount--;
	}

	void Board::render() {
		for ( int y = 0; y < TILE_ROWS; ++y ) {
			for ( int x = 0; x < TILE_COLS; ++x ) {
				mTiles[x][y]->render();
			}
		}
	}

	void Board::reset() {
		for ( int y = 0; y < TILE_ROWS; ++y ) {
			for ( int x = 0; x < TILE_COLS; ++x ) {
				if ( mTiles[x][y]->reset() ) {
					//increment pellet count
					mPelletCount++;
				}
			}
		}
	}

	//get the nearest four neighbors of a tile
	std::vector<Tile*> Board::getTileNeighbors( Tile * tile ) {
		int x = tile->getPosition().getX();
		int y = tile->getPosition().getY();
		std::vector<Tile*> vec;
		if ( Tile * west = getTile( x - 1, y ) ) {
			vec.push_back( west );
		}
		if ( Tile * north = getTile( x, y - 1 ) ) {
			vec.push_back( north );
		}
		if ( Tile * east = getTile( x + 1, y ) ) {
			vec.push_back( east );
		}
		if ( Tile * south = getTile( x, y + 1 ) ) {
			vec.push_back( south );
		}
		return vec;
	}

	bool Board::getClosestFreeTilePosition( int &x, int &y ) {
		//success flag
		bool success = false;
		//perform a breadth first search to find the path to the goal
		BFS<FreeTileSearchNode> bfs(TILE_COLS, TILE_ROWS);
		FreeTileSearchNode start( getTile( x, y ) );
		//there is no particular goal node
		FreeTileSearchNode goal( NULL );
		bfs.setStartAndGoalStates( start, goal );
		bfs.search();
		if ( bfs.foundPath() ) {
			//get the found goal node
			FreeTileSearchNode * goalNode = bfs.getSolutionGoal();
			if ( goalNode ) {
				//get its position
				TilePosition pos = goalNode->getNodePosition();
				x = pos.getX();
				y = pos.getY();
				success = true;
			}
		}
		//free resources
		bfs.clear();
		return success;
	}

	bool Board::outOfBounds( int x, int y ) {
		if ( x < 0 || y < 0 || x >= TILE_COLS || y >= TILE_ROWS ) {
			return true;
		}
		return false;
	}

	bool Board::isTileDeadEnd( TilePosition pos ) {
		return isTileDeadEnd( pos.getX(), pos.getY() );
	}

	bool Board::isTileDeadEnd( int x, int y ) {
		//count number of blocked directions - 3 means dead end
		int blockCount = 0;
		//check left
		if ( outOfBounds( x - 1, y ) || !getTile( x - 1, y )->isFloorTile( true ) ) {
			blockCount++;
		}
		//check right
		if ( outOfBounds( x + 1, y ) || !getTile( x + 1, y )->isFloorTile( true ) ) {
			blockCount++;
		}
		//check up
		if ( outOfBounds( x, y - 1 ) || !getTile( x, y - 1 )->isFloorTile( true ) ) {
			blockCount++;
		}
		//check down
		if ( outOfBounds( x, y + 1 ) || !getTile( x, y + 1 )->isFloorTile( true ) ) {
			blockCount++;
		}
		if ( blockCount >= 3 ) {
			return true;
		}
		return false;
	}

}