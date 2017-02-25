#include "TextureManager.h"

namespace pacman {

	/*
		public methods
	*/
	void TextureManager::freeTextures() {
		//delete tile texture
		if ( mTileTexture != NULL ) {
			delete mTileTexture;
			mTileTexture = NULL;
		}
		//delete pacman texture
		if ( mPacmanTexture != NULL ) {
			delete mPacmanTexture;
			mPacmanTexture = NULL;
		}
		//delete ghosts texture
		if ( mEnemiesTexture != NULL ) {
			delete mEnemiesTexture;
			mEnemiesTexture = NULL;
		}
		//delete texts texture
		if ( mTextsTexture != NULL ) {
			delete mTextsTexture;
			mTextsTexture = NULL;
		}
	}

	WTexture * TextureManager::getTileTexture() {
		//lazy loading of texture
		if ( mTileTexture == NULL ) {
			mTileTexture = new WTexture();
			//mTileTexture->load( DATA_DIR + IMAGE_SUB_DIR + TILE_FILE_NAME );
			mTileTexture->load( TILE_BITMAP_RESOURCE_NAME );
		}
		return mTileTexture;
	}

	WTexture * TextureManager::getPacmanTexture() {
		//lazy loading of texture
		if ( mPacmanTexture == NULL ) {
			mPacmanTexture = new WTexture();
			//mPacmanTexture->load( DATA_DIR + IMAGE_SUB_DIR + PACMAN_FILE_NAME );
			mPacmanTexture->load( PACMAN_BITMAP_RESOURCE_NAME );
		}
		return mPacmanTexture;
	}

	WTexture * TextureManager::getEnemiesTexture() {
		//lazy loading of texture
		if ( mEnemiesTexture == NULL ) {
			mEnemiesTexture = new WTexture();
			//mEnemiesTexture->load( DATA_DIR + IMAGE_SUB_DIR + ENEMIES_FILE_NAME );
			mEnemiesTexture->load( GHOSTS_BITMAP_RESOURCE_NAME );
		}
		return mEnemiesTexture;
	}

	WTexture * TextureManager::getTextsTexture() {
		//lazy loading of texture
		if ( mTextsTexture == NULL ) {
			mTextsTexture = new WTexture();
			//mTextsTexture->load( DATA_DIR + IMAGE_SUB_DIR + TEXTS_FILE_NAME );
			mTextsTexture->load( TEXTS_BITMAP_RESOURCE_NAME );
		}
		return mTextsTexture;
	}

	/*
		private methods
	*/
	TextureManager::~TextureManager() {
		//free resources
		freeTextures();
	}
}