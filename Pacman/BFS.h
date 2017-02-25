#pragma once
#include <queue>
#include <unordered_set>
#include "Constants.h"

template <class T> class SearchState;

template <class UserState> class BFS {

public:
	/*
		public methods
	*/

	//constructor
	BFS( int gridWidth, int gridHeight ) : mGridWidth( gridWidth ), mGridHeight( gridHeight ) {
	}

	void setStartAndGoalStates( UserState &start, UserState &goal ) {
		//initialize start and goal nodes
		mStart = new Node();
		mGoal = new Node();
		mStart->mUserState = start;
		mGoal->mUserState = goal;
		//add start node to queue
		mOpen.push( mStart );
		//mark goal node as closed for memory freeing later
		mClosed.push_back( mGoal );
		//check if the visited array has been initialized
		if ( mVisited == NULL ) {
			initVisitedArray();
		}
	}

	void search() {
		if ( mStart->mUserState.isSameState( mGoal->mUserState ) ) {
			//nothing to search for - start and goal are the same node
			return;
		}
		//mark start node as visited
		mVisited[mStart->mUserState.getX()][mStart->mUserState.getY()] = true;
		while ( !mOpen.empty() ) {
			//get first node from the queue
			Node * node = mOpen.front();
			//remove node from open list
			mOpen.pop();
			//add it to closed list
			mClosed.push_back( node );
			//if it is goal, finish
			if ( node->mUserState.isGoal( mGoal->mUserState ) ) {
				//the goal is the current node
				mGoal = node;
				//start marking path
				mGoal->parent = node->parent;
				//set success flag
				mSuccess = true;
				//set children of nodes along the path from start to goal
				Node * nodeChild = mGoal;
				Node * nodeParent = mGoal->parent;
				do {
					nodeParent->child = nodeChild;
					nodeChild = nodeParent;
					nodeParent = nodeParent->parent;
				} while ( nodeChild != mStart );
				return;
			}
			//else get its successors and add them to the queue
			node->mUserState.getSuccessors( this );
			for ( Node * s : mSuccessors ) {
				//set parent of successors to node
				s->parent = node;
				//push successors into queue
				mOpen.push( s );
				//mark them as visited
				mVisited[s->mUserState.getX()][s->mUserState.getY()] = true;
			}
			//clear the successors list
			mSuccessors.clear();
		}

	}

	void addSuccessor( UserState &state ) {
		//check if this node has already been visited
		if ( mVisited[state.getX()][state.getY()] ) {
			return;
		}
		//create  and add the new node
		Node * node = new Node();
		node->mUserState = state;
		mSuccessors.push_back( node );
	}
	
	void setVisited( int x, int y ) {
		mVisited[x][y] = true;
	}

	bool foundPath() {
		return mSuccess;
	}

	void clear() {
		//remove all from mOpen
		while ( !mOpen.empty() ) {
			//get first node
			Node * node = mOpen.front();
			//remove from queue
			mOpen.pop();
			//delete the node
			delete node;
		}
		//remove all from mClosed
		for ( Node * node : mClosed ) {
			delete node;
		}
		mClosed.clear();
		//start and end nodes are deleted - take care of dangling pointers
		mStart = NULL;
		mGoal = NULL;
		//reset success flag
		mSuccess = false;
		//deallocate the visited array
		for ( int i = 0; i < mGridWidth; i++ ) {
			delete[] mVisited[i];
		}
		delete[] mVisited;
		mVisited = NULL;
	}

	UserState * getSolutionStart() {
		mCurrentSolutionNode = mStart;
		if ( mStart ) {
			return &mStart->mUserState;
		}
		return NULL;
	}

	UserState * getNextSolutionNode() {
		if ( mCurrentSolutionNode ) {
			if ( mCurrentSolutionNode->child ) {
				mCurrentSolutionNode = mCurrentSolutionNode->child;
				return &mCurrentSolutionNode->mUserState;
			}
		}
		return NULL;
	}

	UserState * getSolutionGoal() {
		if ( mGoal ) {
			return &mGoal->mUserState;
		}
		return NULL;
	}

private:
	/*
		private data
	*/

	//a helper node class
	class Node {
	public:
		//used after the search to find the solution path
		Node * parent;
		//used after search to mark the children along the found path
		Node * child;
		Node() {
			parent = NULL;
			child = NULL;
		}
		UserState mUserState;
	};
	//the main queue
	std::queue<Node*> mOpen;
	//keeping pointers to discarded nodes for cleaning up
	std::vector<Node*> mClosed;
	//create a set to keep track of visited nodes
	bool **mVisited = NULL;
	//keep track of the width and height of the two-dimensional grid
	int mGridWidth;
	int mGridHeight;
	Node * mStart;
	Node * mGoal;
	Node * mCurrentSolutionNode;
	//a vector for temporary storage during expansion of nodes
	std::vector< Node * > mSuccessors;
	//flag for successful search
	bool mSuccess = false;

	/*
		private methods
	*/

	void initVisitedArray() {
		//create a two-dimensional array to keep track of visited cells
		mVisited = new bool *[mGridWidth];
		for ( int i = 0; i < mGridWidth; i++ ) {
			mVisited[i] = new bool[mGridHeight]();
		}
	}

};

template <class T> class SearchState {
public:
	virtual ~SearchState() {}
	//returns true if this node is the goal node
	virtual bool isGoal( T &nodeGoal ) = 0;
	//returns true if this node is equal to other
	virtual bool isSameState( T &other ) = 0;
	//retrieves all successors to this node and adds them via bfs.addSuccessor()
	virtual void getSuccessors( BFS<T> *bfs ) = 0;
	//get the x and y coordinates of this node on the grid
	virtual int getX() = 0;
	virtual int getY() = 0;
};