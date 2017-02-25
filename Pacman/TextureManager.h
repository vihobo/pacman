#pragma once
#include "Constants.h"
#include "WTexture.h"

namespace pacman {
	class TextureManager {
	public:
		/*
			public methods
		*/
		static TextureManager & getInstance() {
			static TextureManager instance;
			return instance;
		}
		TextureManager( TextureManager const&) = delete;
		void operator=( TextureManager const &) = delete;

		void freeTextures();
		WTexture * getTileTexture();
		WTexture * getPacmanTexture();
		WTexture * getEnemiesTexture();
		WTexture * getTextsTexture();
	private:
		/*
			private methods
		*/
		TextureManager() {}
		~TextureManager();

		/*
			private member variables
		*/
		WTexture * mTileTexture = NULL;
		WTexture * mPacmanTexture = NULL;
		WTexture * mEnemiesTexture = NULL;
		WTexture * mTextsTexture = NULL;
	};
}