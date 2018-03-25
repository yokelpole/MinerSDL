/*
 main.cpp
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
 The main file, which initializes the core areas of the program.
*/

#include "SDL/SDL.h"

#include "classes.h"
#include "sdl_functions.h"

#include "town_functions.h"
#include "startup_screen.h"
#include "change_working_directory.h"

#include <iostream>

int main(int argc, char* args[])
{
#ifdef __APPLE__
    // Set path to resources folder if computer is on MacOS.
    change_directory_macos();
#endif
    
	// Initialize SDL
	if( SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return -1;
	}
	
	// Initialize the SDL_Objects object.
	// This will also generate the program window via the class
	// constructor.
	SDL_Objects sdl;
	
	// Set the window's caption.
	SDL_WM_SetCaption("Miner SDL", NULL);
	
	// Make the player's and mine's objects.
	PlayerData *player = new PlayerData;
	MineData *mine = new MineData;
	
	// Load the welcoming screen.
	// Take control from main();
	while(!sdl.return_quitSDL())
	{
		if(sdl.return_quit_to_menu())
		{		
			// Clear out the old player and mine instances.
			delete player;
			delete mine;
			
			// Clear out the status information log.
			sdl.clear_status_text();
			
			// Start a new player and mine instance.
			player = new PlayerData;
			mine = new MineData;
			
			startup_screen(player, mine, &sdl);
		}
		else
		{
			main_town(player, mine, &sdl);
		}
	}
	
	return 0;
}
