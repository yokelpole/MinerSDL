/*
 tavern.h
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
 Functions dealing with the tavern section of the game.
 
 Here the player can see Mimi or get a tip as to where the diamond is.
 
 The game can be finished from this area, as if the player meets
 conditions desirable to Mimi ($2500+ & diamond in possession) the
 game will end.
 */

#ifndef TAVERN_FUNCTIONS
#define TAVERN_FUNCTIONS

#include "SDL/SDL.h"
#include "classes.h"
#include "sdl_functions.h"

// Enumerations for the amount of tip chosen.
enum tip_amount
{
	CHEAP,
	GOOD,
	BEST
};

// The main function for the tavern
void tavern(PlayerData *player, MineData *mine, SDL_Objects *sdl);

class Tavern_Objects
{
	private:
		// The graphics for the tavern.
		SDL_Surface *tavern_graphic;
		SDL_Surface *tavern_transparency;
		SDL_Surface *see_mimi_button;
		SDL_Surface *cheap_tip_button;
		SDL_Surface *good_tip_button;
		SDL_Surface *best_tip_button;
		SDL_Surface *exit_tavern_button;
		SDL_Surface *arrow_graphic;
		
		// The graphics for the minimap
		SDL_Surface *minimap;
		SDL_Surface *minimap_big_overlay;
		SDL_Surface *minimap_medium_overlay;
		SDL_Surface *minimap_small_overlay;
		SDL_Surface *minimap_explored_area;
		
		// Graphics for Mimi
		SDL_Surface *mimi_happy;
		SDL_Surface *mimi_sad;
		SDL_Surface *mimi_talk_window;
		
		// Fonts for the screen.
		TTF_Font *header_font;
		TTF_Font *display_font;
		
	public:
		Tavern_Objects();
		~Tavern_Objects();
		
		// Update the objects on the screen.
		void update_tavern_graphics(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *hospital_arrow);
		void update_tavern_graphics_no_overlay(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *hospital_arrow);

		// Animate the movement of the arrow.
		void animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection);
		void animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Functions that respond to player actions.
		bool see_mimi(PlayerData *player, SDL_Objects *sdl, MineData *mine);
		void get_tip(PlayerData *player, MineData *mine, SDL_Objects *sdl, tip_amount tip);

		// Show the player the map of where the diamond is.
		void show_map(MineData *mine, SDL_Objects *sdl);

		// Display the tip on the map. 
		void display_tip(MineData *mine, SDL_Objects *sdl, tip_amount tip);
		
		// Function that waits for the player to press enter.
		void wait_for_enter(SDL_Objects *sdl);
};

#endif
