#include "TilePosition.h"
#include <cmath>

namespace pacman {

	TilePosition::TilePosition( int x, int y ) {
		mXIndex = x;
		mYIndex = y;
	}

	void TilePosition::setX( int x ) {
		mXIndex = x;
	}

	void TilePosition::setY( int y ) {
		mYIndex = y;
	}

	int TilePosition::getX() const {
		return mXIndex;
	}

	int TilePosition::getY() const {
		return mYIndex;
	}

	int TilePosition::getManhattanDistanceTo( const TilePosition& other ) const {
		int dx = std::abs( this->mXIndex - other.getX() );
		int dy = std::abs( this->mYIndex - other.getY() );
		return dx + dy;
	}

	TilePosition TilePosition::diff( const TilePosition &other ) {
		int x = this->getX() - other.getX();
		int y = this->getY() - other.getY();
		return TilePosition( x, y );
	}
}