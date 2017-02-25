#pragma once
#include "Constants.h"

namespace pacman {
	class Scoreboard {
	public:
		/*
			public methods
		*/
		static Scoreboard & getInstance() {
			static Scoreboard instance;
			return instance;
		}
		Scoreboard( Scoreboard const& ) = delete;
		void operator=( Scoreboard const & ) = delete;

		void render() const;
		void reset();
		void increaseScore(int amount);
		void loseLife();
	private:
		/*
			private methods
		*/
		Scoreboard() {}
		void renderLives() const;
		void renderScore() const;

		/*
			private member variables
		*/
		int mScore = 0;
		int mLivesLeft = PLAYER_LIFE_COUNT;
	};
}