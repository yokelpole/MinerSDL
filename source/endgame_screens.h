/*
 endgame_screens.h
 ---------------------------------------
 This file is a part of MinerSDL.
 MinerSDL is Copyright Kyle Poole, 2011.
 ---------------------------------------
 MinerSDL is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 MinerSDL is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with MinerSDL.  If not, see <http://www.gnu.org/licenses/>.
 --------------------------------------- 
 Functions that deal with the display of Mimi's victory screens.
 */

#ifndef ENDGAME_SCREENS
#define ENDGAME_SCREENS

#include "SDL/SDL.h"
#include "classes.h"

// Call the ending.
void display_ending(SDL_Objects *sdl, PlayerData *player, MineData *mine);

class Endgame_Screen_Data
{
	private:
		SDL_Surface *good_background;
		SDL_Surface *bad_background;
		
		TTF_Font *big_header_font;
		TTF_Font *header_font;
		TTF_Font *standard_font;
	
	public:
		Endgame_Screen_Data();
		~Endgame_Screen_Data();
		
		// Refresh the good ending screen.
		void display_good_ending_screen(SDL_Objects *sdl, PlayerData *player, MineData *mine);
		
		// Refresh the bad ending screen.
		void display_bad_ending_screen(SDL_Objects *sdl, PlayerData *player, MineData *mine);
};

#endif
