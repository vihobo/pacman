#pragma once
#include "SDL.h"
#include "Character.h"

namespace pacman {
	class Player : public Character {
	public:
		/*
			public methods
		*/
		Player( int x, int y );
		Player( TilePosition pos );
		void handleEvent( SDL_Event& e );
		void move();
		void render();
		void die();
		void reset();
		bool isDying() const;
	private:
		/*
			private member variables
		*/
		bool mIsDying = false;
		int mFrameCounter = 0;
	};
}