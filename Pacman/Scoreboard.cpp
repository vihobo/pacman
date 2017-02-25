#include "Scoreboard.h"
#include "Game.h"

namespace pacman {

	/*
		public methods
	*/
	void Scoreboard::render() const {
		renderLives();
		renderScore();
	}

	void Scoreboard::reset() {
		//reset score
		mScore = 0;
		//reset lives
		mLivesLeft = PLAYER_LIFE_COUNT;
	}

	void Scoreboard::increaseScore( int amount ) {
		mScore += amount;
	}

	void Scoreboard::loseLife() {
		mLivesLeft--;
		if ( mLivesLeft <= 0 ) {
			//game lost
			Game::getInstance().lose();
		}
	}

	/*
		private methods
	*/
	void Scoreboard::renderLives() const {
		//render LIVES: text
		SDL_Rect clip = { 0, LETTER_HEIGHT, 6 * LETTER_WIDTH, LETTER_HEIGHT };
		TextureManager::getInstance().getTextsTexture()->render( TEXT_PADDING, TILE_ROWS * TILE_SIZE + TEXT_PADDING, &clip );
		//render the number of lives left
		//clip = { mLivesLeft * LETTER_WIDTH, 0, LETTER_WIDTH, LETTER_HEIGHT };
		clip.x = mLivesLeft * LETTER_WIDTH;
		clip.y = 0;
		clip.w = LETTER_WIDTH;
		TextureManager::getInstance().getTextsTexture()->render( TEXT_PADDING + 6 * LETTER_WIDTH, TILE_ROWS * TILE_SIZE + 5, &clip );
	}

	void Scoreboard::renderScore() const {
		//render SCORE: text
		SDL_Rect clip = { 0, 2 * LETTER_HEIGHT, 6 * LETTER_WIDTH, LETTER_HEIGHT };
		//position text so that there is room for 5 digits to the right and padding
		int xPosition = SCREEN_WIDTH - 5 * LETTER_WIDTH - 6 * LETTER_WIDTH - TEXT_PADDING;
		TextureManager::getInstance().getTextsTexture()->render( xPosition, TILE_ROWS * TILE_SIZE + TEXT_PADDING, &clip );
		//render the score number always padded to 5 digits
		int score = mScore;
		//position of rightmost digit
		xPosition = SCREEN_WIDTH - LETTER_WIDTH - TEXT_PADDING;
		for ( int i = 1;i <= 5;i++ ) {
			int digit = 0;
			if ( score ) {
				digit = score % 10;
				score /= 10;
			}
			clip.x = digit * LETTER_WIDTH;
			clip.y = 0;
			clip.w = LETTER_WIDTH;
			clip.h = LETTER_HEIGHT;
			TextureManager::getInstance().getTextsTexture()->render( xPosition, TILE_ROWS * TILE_SIZE + TEXT_PADDING, &clip );
			xPosition -= LETTER_WIDTH;
		}
	}

}