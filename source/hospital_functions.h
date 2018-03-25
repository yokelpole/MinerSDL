/*
 hospital_functions.h
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
 Functions pertaining to the hospital.
 
 The hospital is where the player can go to refill health or
 purchase insurance.
*/

#ifndef HOSPITAL_FUNCTIONS
#define HOSPITAL_FUNCTIONS

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "classes.h"
#include "sdl_functions.h"

// Function to load the hospital screen.
void hospital(PlayerData *player, SDL_Objects *sdl);

// The hospital's class and associated variables.
class Hospital_Objects
{
	private:
		SDL_Surface *hospital_graphic;
		SDL_Surface *hospital_arrow_graphic;
		SDL_Surface *one_day_button;
		SDL_Surface *full_heal_button;
		SDL_Surface *insurance_button;
		SDL_Surface *exit_button;
		
		TTF_Font *header_font;
		TTF_Font *display_font;
	public:
		Hospital_Objects();	
		~Hospital_Objects();
		
		// Update the objects on the screen.
		void update_hospital_graphics(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *hospital_arrow);

		// Apply the hospital's treatments to ther player's stats.
		void stay_one_day(PlayerData *player);
		void full_heal(PlayerData *player);
		void insurance(PlayerData *player);
		
		// Animate the movement of the arrow.
		void animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection);
		void animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection);
};

#endif
