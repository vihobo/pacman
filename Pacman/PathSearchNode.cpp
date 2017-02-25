#include "PathSearchNode.h"
#include "Board.h"

namespace pacman {

	/*
		public methods
	*/

	PathSearchNode::PathSearchNode( Tile * tile ) {
		mTile = tile;
	}

	// returns true if this node is the goal node
	bool PathSearchNode::isGoal( PathSearchNode &nodeGoal ) {
		return isSameState( nodeGoal );
	}

	bool PathSearchNode::isSameState( PathSearchNode &other ) {
		if ( mTile == other.mTile ) {
			return true;
		}
		return false;
	}

	// retrieves all successors to this node and adds them via bfs.addSuccessor()
	void PathSearchNode::getSuccessors( BFS<PathSearchNode> *bfs ) {
		//get neighbours
		std::vector<Tile*> vec = Board::getInstance().getTileNeighbors( mTile );
		for ( Tile * tile : vec ) {
			//check if tile is traversable
			if ( tile->isFloorTile(true) ) {
				//create nodes and add each one to the search
				PathSearchNode node(tile);
				bfs->addSuccessor( node );
			}
		}
	}

	int PathSearchNode::getX() {
		return mTile->getPosition().getX();
	}

	int PathSearchNode::getY() {
		return mTile->getPosition().getY();
	}

	TRAVEL_DIRECTION PathSearchNode::getDirection( TilePosition fromPos ) {
		TilePosition diff = mTile->getPosition().diff( fromPos );
		int x = diff.getX();
		int y = diff.getY();
		if ( x == 0 ) {
			return y == 1 ? TRAVEL_DOWN : TRAVEL_UP;
		}
		if ( y == 0 ) {
			return x == 1 ? TRAVEL_RIGHT : TRAVEL_LEFT;
		}
		return TRAVEL_DEFAULT;
	}
}