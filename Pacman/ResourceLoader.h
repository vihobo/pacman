#pragma once

#include <SDL.h>
#include <windows.h>
#include <cstdio>
#include <iostream>
#include <istream>
#include <streambuf>
#include <string>

namespace pacman {

	struct membuf : std::streambuf {
		membuf( char* begin, char* end ) {
			this->setg( begin, begin, end );
		}
	};

	class ResourceLoader {
	public:
		//loads a resource file
		static void LoadFileInResource( int name, int type, DWORD& size, const char*& data );
		//creates an SDL_Surface * from resource file
		static SDL_Surface * ResourceLoader::SDL_LoadBMPFromResource( HINSTANCE hInst, LPCTSTR lpResName );
	};
}