#include "WTexture.h"
#include "ResourceLoader.h"
#include "Game.h"

namespace pacman {

	WTexture::WTexture() {
		//initialize variables
		mWidth = 0;
		mHeight = 0;
		mTexture = NULL;
	}

	WTexture::~WTexture() {
		//clean up
		free();
	}

	//bool WTexture::load( std::string pathToFile ) {
	bool WTexture::load( int bitmapName ) {
		//remove any remaining old texture
		free();
		//load image
		SDL_Surface * surface = ResourceLoader::SDL_LoadBMPFromResource(GetModuleHandle(NULL), MAKEINTRESOURCE( bitmapName ));
		//SDL_Surface * surface = SDL_LoadBMP( pathToFile.c_str() );
		if ( surface != NULL ) {
			//color key image
			SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF ) );
			//create texture from surface
			mTexture = SDL_CreateTextureFromSurface( Game::getInstance().getRenderer(), surface );
			if ( mTexture == NULL ) {
				printf( "Unable to create texture from surface! SDL Error: %s\n", SDL_GetError() );
			} else {
				//get the width and height of the loaded image
				mWidth = surface->w;
				mHeight = surface->h;
			}
			//dispose of the created surface
			SDL_FreeSurface( surface );
		} else {
			//printf( "Unable to create texture from file %s! SDL Error: %s\n", pathToFile.c_str(), SDL_GetError() );
			printf( "Unable to create texture from file %d! SDL Error: %s\n", bitmapName, SDL_GetError() );
		}
		//return whether or not the loading succeeded
		return mTexture != NULL;
	}

	void WTexture::free() {
		//deallocate the texture if not NULL
		if ( mTexture != NULL ) {
			SDL_DestroyTexture( mTexture );
			mTexture = NULL;
		}
		//reset the width and height
		mWidth = 0;
		mHeight = 0;
	}

	void WTexture::render( int x, int y, SDL_Rect * clip, double angle, const SDL_Point * center, SDL_RendererFlip flip ) const {
		//set position and dimensions
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };
		if ( clip != NULL ) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}
		//render to screen
		SDL_RenderCopyEx( Game::getInstance().getRenderer(), mTexture, clip, &renderQuad, angle, center, flip );
	}

	int WTexture::getWidth() const {
		return mWidth;
	}

	int WTexture::getHeight() const {
		return mHeight;
	}
}