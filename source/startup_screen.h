/*
 startup_screen.h
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
 Screen shown when the program first starts.
 
 Allows player to start a new game, load a game, see high scores
 or to quit the program.
 */

#ifndef STARTUP_SCREEN
#define STARTUP_SCREEN

#include "SDL/SDL.h"
#include "classes.h"
#include "sdl_functions.h"

void startup_screen(PlayerData *player, MineData *mine, SDL_Objects *sdl);

class Start_Screen
{
	private:
		SDL_Surface *background;
		SDL_Surface *miner_sdl_logo;
		SDL_Surface *new_game_button;
		SDL_Surface *load_game_button;
		SDL_Surface *high_score_button;
        SDL_Surface *instructions_button;
        SDL_Surface *copyright_info;
		SDL_Surface *exit_game_button;
		
		SDL_Surface *arrow;
	
	public:
		Start_Screen();
		~Start_Screen();
	
		// Update the display.
		void update_start_screen(SDL_Objects *sdl, Selection_Arrow *selection);
	
		// Animate the movement of the arrow.
		void animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection);
		void animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection);	
};

#endif
