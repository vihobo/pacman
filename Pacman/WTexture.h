//a wrapper class for the used textures that deals 
//with loading SDL_Surface objects and converting
//them to SDL_Texture objects

#pragma once
#include <SDL.h>
#include <string>

namespace pacman {
	class WTexture {
	public:
		/*
			public methods
		*/
		WTexture();
		~WTexture();
		//bool load( std::string pathToFile );
		bool load( int bitmapName );
		void free();
		void render( int x, int y, SDL_Rect * clip = NULL, double angle = 0.0, const SDL_Point * center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE ) const;
		int getWidth() const;
		int getHeight() const;
	private:
		/*
			private member variables
		*/
		SDL_Texture * mTexture;
		int mWidth;
		int mHeight;
	};
}