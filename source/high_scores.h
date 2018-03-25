/*
 high_scores.h
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
 Functions that deal with the display and writing of high scores.
 
 All information will be saved in filename "high_scores"
*/

#ifndef HIGH_SCORES
#define HIGH_SCORES

// Used for access to string libraries, etc.
#include <string>
#include <sstream>

// Used for the writing/accessing of files on the drive.
#include <iostream>
#include <fstream>

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "classes.h"

// Display the high scores.
void display_high_scores(SDL_Objects *sdl, PlayerData *player, bool high_score_entry);

class High_Score_Objects
{
	private:
		// Graphics for the scoreboard
		SDL_Surface *background;
		SDL_Surface *name_entry;
		SDL_Surface *diamond_graphic;
		SDL_Surface *mimi_happy_graphic;
		SDL_Surface *mimi_sad_graphic;
		SDL_Surface *headstone_graphic;
		
		TTF_Font *header_font;
		TTF_Font *standard_font;
		
		// Variables used for storage of items from the high score file.
		std::string player_name[5];
		std::string high_score_name;
		int player_money[5];
		bool had_diamond[5];
		bool player_dead[5];
		
	public:
		// Initializer etc.
		High_Score_Objects();
		~High_Score_Objects();
		
		// Update the graphics on screen.
		void update_high_score_graphics(SDL_Objects *sdl);
	
		// Allow the user to enter name for entry into scoreboard
		void name_entry_for_high_score(SDL_Objects *sdl, PlayerData *player);
		
		// Update high score information
		void write_high_scores();
		
		// Load the high scores.
		void load_high_scores();
		
		// Check to see if a high score can be entered into the current set.
		bool check_high_score(PlayerData *player);
		
		// Rearrange the high scores once one has been entered.
		void rearrange_high_scores();		

		// Wait for a user keypress to exit the high score screen.
		void wait_for_keypress(SDL_Objects *sdl);
};

#endif
