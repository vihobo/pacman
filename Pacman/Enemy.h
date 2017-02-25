#pragma once
#include "Character.h"

namespace pacman {
	enum EnemyState {
		ENEMY_INITIAL_STATE,
		ENEMY_SCATTER_STATE,
		ENEMY_CHASE_STATE,
		ENEMY_VULNERABLE_STATE,
		ENEMY_DEAD_STATE,
		ENEMY_DEFAULT_STATE
	};
	class Enemy : public Character {
	public:
		/*
			public methods
		*/
		Enemy( int index );
		void init();
		void render();
		void move();
		void collideWithPlayer();
		void setPlayerPosition( TilePosition pos );
		void becomeVulnerable();
		void reset();
	private:
		/*
			private member variables
		*/
		//flip counter
		int mFlipCount = 0;
		//whether ghost has been eaten
		bool mIsDead = false;
		//the ghost id
		int mId;
		//the corner for the last scatter phase
		int mScatterId;
		EnemyState mState;
		//flag to decide whether ghost should be stationary
		bool mIsMoving = false;
		//flag indicating whether ghost is moving towards target
		//initially, it is not
		bool mMoveToTarget = false;
		//start time of current phase
		Uint32 mPhaseStart;
		TilePosition mCurrentTarget;

		/*
			private methods
		*/
		//set ghost as dead
		void die();
		//try to change state if target has been reached
		void calculateChangeState();
		//check whether current phase is over
		bool Enemy::isPhaseOver() const;
		//put enemy in scatter state
		void Enemy::scatter();
		//put enemy in chase state
		void Enemy::chase();
		//calculate new direction of travel
		void calculateNewDirection();
		//find a way out of dead ends
		void resolveDeadEnds();
		//check if ghost can travel in this direction
		bool canTravelInDirection( TRAVEL_DIRECTION dir );
		//get the more promising one of the currently best direction and dirToCheck
		void Enemy::getShorterPath( TRAVEL_DIRECTION dirToCheck, TRAVEL_DIRECTION &bestDir, int &minDistance ) const;
		//calculate scatter target for this ghost
		TilePosition getScatterTarget( int id ) const;
		//find the closest free tile to a ghost's scatter target
		bool Enemy::getFreeScatterTarget( int id, TilePosition &result );
		//whether the ghost is still in the starting pen
		inline bool isInInitialPhase() const { return mState == ENEMY_INITIAL_STATE; }
	};
}