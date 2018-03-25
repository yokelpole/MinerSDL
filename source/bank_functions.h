/*
 bank_functions.h
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
 Functions pertaining to the bank.
 
 The bank is where the player can go to sell off minerals they
 have acquired in the mine. 
 */

#ifndef BANK_FUNCTIONS
#define BANK_FUNCTIONS

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"

#include "classes.h"
#include "sdl_functions.h"

// Load the bank
void bank(PlayerData *player, SDL_Objects *sdl);

// The bank's class and its associated variables.
class Bank_Objects
{
	private:
		SDL_Surface *bank_graphic;
		SDL_Surface *bank_arrow_graphic;
		SDL_Surface *sell_coal_graphic;
		SDL_Surface *sell_silver_graphic;
		SDL_Surface *sell_gold_graphic;
		SDL_Surface *sell_platinum_graphic;
		SDL_Surface *sell_all_graphic;
		SDL_Surface *exit_graphic;
		
		SDL_Surface *platinum_graphic;
		SDL_Surface *gold_graphic;
		SDL_Surface *silver_graphic;
		SDL_Surface *coal_graphic;
		
		TTF_Font *header_font;
		TTF_Font *display_font;
		
	public:
		Bank_Objects();
		
		~Bank_Objects();
		
		// Below functions deal with the selling of materials to bank.
		void bank_sell_all(PlayerData *player);		
		void bank_sell_platinum(PlayerData *player);		
		void bank_sell_gold(PlayerData *player);		
		void bank_sell_silver(PlayerData *player);		
		void bank_sell_coal(PlayerData *player);
		
		// Below deals with randomizing values of minerals.
		// Checks if enough turns have happened to randomize them.
		void bank_randomize_values(PlayerData *player);

		// Below upates the bank graphics on screen.
		void update_bank_graphics(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *bank_arrow);
		
        // Update the main items on the screen. Allows for animation.
    void display_bank_information(SDL_Objects *sdl, PlayerData *player); 
    
		// Animate the movement of the selection arrow.
		void animate_arrow_up(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *selection);
		void animate_arrow_down(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *selection);
};

#endif
