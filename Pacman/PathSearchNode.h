#pragma once
#include "Tile.h"
#include "BFS.h"
#include "Character.h"

namespace pacman {

	class PathSearchNode : SearchState<PathSearchNode> {
	public:
		/*
			public methods
		*/
		PathSearchNode() {};
		PathSearchNode( Tile * tile );
		virtual ~PathSearchNode() {}
		// returns true if this node is the goal node
		virtual bool isGoal( PathSearchNode &nodeGoal );
		bool isSameState( PathSearchNode &other );
		// retrieves all successors to this node and adds them via bfs.addSuccessor()
		virtual void getSuccessors( BFS<PathSearchNode> *bfs );
		//return the x and y position of the node on the grid
		int getX();
		int getY();
		TRAVEL_DIRECTION getDirection( TilePosition fromPos );
	protected:
		/*
			protected member variables
		*/
		Tile * mTile;
	};
}