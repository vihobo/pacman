#pragma once
#include "PathSearchNode.h"

namespace pacman {

	class FreeTileSearchNode : public PathSearchNode {
	public:
		/*
			public methods
		*/
		//constructors
		FreeTileSearchNode() {};
		FreeTileSearchNode( Tile * tile );
		// returns true if this node is the goal node
		bool isGoal( FreeTileSearchNode &nodeGoal );
		// retrieves all successors to this node and adds them via bfs.addSuccessor()
		void getSuccessors( BFS<FreeTileSearchNode> *bfs );
		TilePosition getNodePosition();
	};

}