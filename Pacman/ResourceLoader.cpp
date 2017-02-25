#include "ResourceLoader.h"

namespace pacman {

	//from http://stackoverflow.com/questions/2933295/embed-text-file-in-a-resource-in-a-native-windows-application
	void ResourceLoader::LoadFileInResource( int name, int type, DWORD& size, const char*& data ) {
		HMODULE handle = ::GetModuleHandle( NULL );
		HRSRC rc = ::FindResource( handle, MAKEINTRESOURCE( name ), MAKEINTRESOURCE( type ) );
		HGLOBAL rcData = ::LoadResource( handle, rc );
		size = ::SizeofResource( handle, rc );
		data = static_cast< const char* >(::LockResource( rcData ));
	}

	//from https://www.gamedev.net/topic/315178-hbitmap-to-sdl_surface/
	SDL_Surface * ResourceLoader::SDL_LoadBMPFromResource( HINSTANCE hInst, LPCTSTR lpResName ) {
		HBITMAP hBitmap;
		BITMAP bm;
		Uint8 *bits = NULL;
		Uint8 *temp = NULL;
		SDL_Surface *surf = NULL;

		//Load Bitmap From the Resource into HBITMAP
		hBitmap = ( HBITMAP )LoadImage( hInst, lpResName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );

		//Now Get a BITMAP structure for the HBITMAP
		GetObject( hBitmap, sizeof( bm ), &bm );

		//create a new surface
		surf = SDL_CreateRGBSurface( SDL_SWSURFACE, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel,
			0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );
		bits = new Uint8[bm.bmWidthBytes*bm.bmHeight];
		temp = new Uint8[bm.bmWidthBytes*bm.bmHeight];
		memcpy( temp, bm.bmBits, bm.bmWidthBytes*bm.bmHeight );

		//unfortunately, both the vertical orientation and colordata are reversed
		//so now we will put them back in the right order
		//first flip image over
		//this is probably not the fastest/best way to do this
		Uint8 *ptemp;
		Uint8 *pbits = bits;
		for ( int j = bm.bmHeight - 1; j >= 0; j-- ) {
			ptemp = temp + j * bm.bmWidthBytes;
			for ( int x = 0; x < bm.bmWidthBytes; x++ ) {
				*pbits = *ptemp;
				pbits++;
				ptemp++;
			}
		}

		//Now reverse BGR data to be RGB
		for ( int i = 0; i < bm.bmWidthBytes*bm.bmHeight; i += 3 ) {
			Uint8 temp;
			temp = bits[i];
			bits[i] = bits[i + 2];
			bits[i + 2] = temp;
		}
		//Now just copy bits onto surface
		if ( SDL_MUSTLOCK( surf ) ) SDL_LockSurface( surf );
		memcpy( surf->pixels, bits, bm.bmWidthBytes*bm.bmHeight );
		if ( SDL_MUSTLOCK( surf ) ) SDL_UnlockSurface( surf );

		delete[] bits;
		delete[] temp;
		//Finally, convert surface to display format so it displays correctly
		//SDL_Surface *surf2 = SDL_DisplayFormat( surf );
		SDL_Surface *surf2 = SDL_ConvertSurfaceFormat( surf, SDL_PIXELFORMAT_RGB888, 0 );
		SDL_FreeSurface( surf );

		return surf2;
		//return surf;

	}
}