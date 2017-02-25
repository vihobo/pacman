#pragma once

namespace pacman {
	class TilePosition {
	public:
		/*
			public methods
		*/
		TilePosition( int x = 0, int y = 0 );
		void setX( int x );
		void setY( int y );
		int getX() const;
		int getY() const;
		int getManhattanDistanceTo( const TilePosition& other ) const;
		inline bool operator==( const TilePosition& rhs ) const {
			return (this->getX() == rhs.getX()) && (this->getY() == rhs.getY());
		}
		inline bool operator!=( const TilePosition& rhs ) const {
			return !operator==( rhs );
		}
		//get difference between two tiles
		TilePosition diff(const TilePosition &other);
	private:
		/*
			private member variables
		*/
		int mXIndex;
		int mYIndex;
	};
}