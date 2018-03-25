/*
 town_functions.h
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
 Functions that deal with the different areas of the town.
 
 The player is able to access the main menu, bank, hospital, store,
 tavern, or the mine. 
 
 The main starting point for a new/loaded game.
*/

#ifndef TOWN_FUNCTIONS
#define TOWN_FUNCTIONS

#include "SDL/SDL.h"
#include "classes.h"
#include "sdl_functions.h"

// Loads and displays the screen for the main town.
void main_town(PlayerData *player, MineData *mine, SDL_Objects *sdl);

// Class to hold the information contained within the town.
class Town_Objects
{
	private:
		// Load the graphics for the town.
		SDL_Surface *town_graphic;
		
		SDL_Surface *bank_graphic;
		SDL_Surface *tavern_graphic;
		SDL_Surface *hospital_graphic;
		SDL_Surface *store_graphic;
		SDL_Surface *mine_graphic;
		
		SDL_Surface *arrow_graphic;
		
		TTF_Font *display_font;
		
	public:
		Town_Objects();
		
		~Town_Objects();
		
		// Update the graphics for the town.
		void update_town_graphics(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Animate the movement of the selection arrow.
		void animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection);
		void animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection);
};

#endif
