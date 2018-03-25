/*
 store_functions.h
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
 Functions that deal with the store.
 
 Here the player is able to purchase various items that will be of 
 aid within the mine.
 */

#ifndef STORE_FUNCTIONS
#define STORE_FUNCTIONS

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "classes.h"
#include "sdl_functions.h"

// Function to load the store screen.
void store(PlayerData *player, SDL_Objects *sdl);

// Class to store all store items.
class Store_Objects
{
	private:
		SDL_Surface *store_graphic;
		SDL_Surface *selection_box;
		SDL_Surface *pickaxe_graphic;
		SDL_Surface *bucket_graphic;
		SDL_Surface *dynamite_graphic;
		SDL_Surface *flashlight_graphic;
		SDL_Surface *hardhat_graphic;
		SDL_Surface *shovel_graphic;
		SDL_Surface *exit_graphic;
		
		TTF_Font *header_font;
		TTF_Font *display_font;

	public:
		Store_Objects();
		
		~Store_Objects();

		// Update the objects on the screen.
		void update_store_graphics(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *store_arrow);
		void update_animated_graphics(SDL_Objects *sdl, PlayerData *player);

		// Animate the movement of the selection box.
		void animate_box_down(SDL_Objects *sdl, Selection_Arrow *selection, PlayerData *player);
		void animate_box_up(SDL_Objects *sdl, Selection_Arrow *selection, PlayerData *player);
		void animate_box_left(SDL_Objects *sdl, Selection_Arrow *selection, PlayerData *player);
		void animate_box_right(SDL_Objects *sdl, Selection_Arrow *selection, PlayerData *player);

};

#endif
