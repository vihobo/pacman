#include "FreeTileSearchNode.h"
#include "Board.h"

namespace pacman {

	/*
		public methods
	*/

	FreeTileSearchNode::FreeTileSearchNode( Tile * tile ) : PathSearchNode( tile ) {}

	// returns true if this node is the goal node
	bool FreeTileSearchNode::isGoal( FreeTileSearchNode & ) {
		//the goal is to find the first free tile
		if ( mTile->isFloorTile() ) {
			return true;
		}
		return false;
	}

	// retrieves all successors to this node and adds them via bfs.addSuccessor()
	void FreeTileSearchNode::getSuccessors( BFS<FreeTileSearchNode> *bfs ) {
		//get neighbours
		std::vector<Tile*> vec = Board::getInstance().getTileNeighbors( mTile );
		for ( Tile * tile : vec ) {
			//create nodes and add each one to the search
			FreeTileSearchNode node( tile );
			bfs->addSuccessor( node );
		}
	}

	TilePosition FreeTileSearchNode::getNodePosition() {
		return mTile->getPosition();
	}

}