/*
 endgame_screens.cpp
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

#include <string>
#include <sstream>

// General SDL includes.
#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"

// General program includes
#include "sdl_functions.h"
#include "classes.h"
#include "timer.h"

#include "endgame_screens.h"

// Call the ending.
void display_ending(SDL_Objects *sdl, PlayerData *player, MineData *mine)
{
	// Create the endgame data.
	Endgame_Screen_Data endgame_data;
	
	// Initialize an event to track the user's input
	SDL_Event user_input;
	
	bool exit = false;		// Keeps track of whether to exit main loop.
	
	// Update the status text.
	sdl->update_status_text("Congratulations, you win!");
	
	// Display the proper screen according to the player's wealth.
	if(player->get_money() < 5000)
	{
		endgame_data.display_bad_ending_screen(sdl, player, mine);
	}
	else
	{
		endgame_data.display_good_ending_screen(sdl, player, mine);
	}
	
	// Update the screen.
	SDL_Flip(sdl->return_screen());
	SDL_Delay(1500);
	
	while(!exit)
	{		
		while(SDL_PollEvent(&user_input))
		{	
			// Quit if the user chooses to close the window.
			if(user_input.type == SDL_QUIT)
			{
				sdl->set_quitSDL();
				exit = true;
			}
			else if(user_input.type == SDL_KEYDOWN)
			{
				exit = true;
			}
		}
	}
}

// Initialize the objects
Endgame_Screen_Data::Endgame_Screen_Data()
{
	good_background = IMG_Load("./Graphics/ending_screen/good_ending.png");
	bad_background = IMG_Load("./Graphics/ending_screen/bad_ending.png");
	
	big_header_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 36);
	header_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
	standard_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 20);
}

// Take the objects out of memory.
Endgame_Screen_Data::~Endgame_Screen_Data()
{
	SDL_FreeSurface(good_background);
	SDL_FreeSurface(bad_background);
	
	TTF_CloseFont(big_header_font);
	TTF_CloseFont(header_font);
	TTF_CloseFont(standard_font);
}

// Refresh the good ending screen.
void Endgame_Screen_Data::display_good_ending_screen(SDL_Objects *sdl, PlayerData *player, MineData *mine)
{
	// Apply the background image.
	sdl->apply_surface(0, 0, good_background, sdl->return_screen());
	
	// Apply the header for the congratulations.
	sdl->apply_text(465, 45, "YOU WIN!", big_header_font, sdl->return_screen());
	
	// Congratulatory text.
	sdl->apply_text(445, 90, "You managed to get", standard_font, sdl->return_screen());
	sdl->apply_text(445, 115, "both the diamond", standard_font, sdl->return_screen());
	sdl->apply_text(445, 140, "and enough money", standard_font, sdl->return_screen());
	sdl->apply_text(445, 165, "to keep Mimi happy!", standard_font, sdl->return_screen());
	
	// Apply the header for the statistics.
	sdl->apply_text(495, 250, "STATS", big_header_font, sdl->return_screen());
	
	// Stats to list: total gold, % of mine explored, # of turns
	sdl->apply_text(445, 300, "Total Gold:", standard_font, sdl->return_screen());
	sdl->apply_text(445, 325, "% Explored:", standard_font, sdl->return_screen());
	sdl->apply_text(445, 350, "# of Turns:", standard_font, sdl->return_screen());
	sdl->apply_text(445, 375, "Health:", standard_font, sdl->return_screen());
	
	// Stores the temporary values for strings being posted on the screen.
	std::string temp_string;
	std::stringstream temp_stringstream;	
	int temp_int = 0;
	
	temp_int = player->get_money();
	temp_stringstream << temp_int;
	temp_string = temp_stringstream.str();
	sdl->apply_text(610, 300, temp_string, standard_font, sdl->return_screen());
	temp_stringstream.str("");
	
	// Compute the amount of the mine that has been explored
	temp_int = 0;
	for(int x = 1; x < mine->get_map_x(); x++)
	{
		for(int y = 0; y < mine->get_map_y(); y++)
		{
			if(mine->get_explored(x, y))
			{
				temp_int++;
			}
		}
	}
	
	// Calculate the percentage of the mine that has been explored.
	temp_int = ((double)temp_int / 36864) * 100;
	temp_stringstream << temp_int << "%";
	temp_string = temp_stringstream.str();
	sdl->apply_text(610, 325, temp_string, standard_font, sdl->return_screen());
	temp_stringstream.str("");	
	
	// State the number of turns taken.
	temp_int = player->get_turn_number();
	temp_stringstream << temp_int;
	temp_string = temp_stringstream.str();
	sdl->apply_text(610, 350, temp_string, standard_font, sdl->return_screen());
	temp_stringstream.str("");
	
	// Display the player's ending health
	temp_int = player->get_health();
	temp_stringstream << temp_int;
	temp_string = temp_stringstream.str();
	sdl->apply_text(610, 375, temp_string, standard_font, sdl->return_screen());
	temp_stringstream.str("");
	
}

// Refresh the bad ending screen.
void Endgame_Screen_Data::display_bad_ending_screen(SDL_Objects *sdl, PlayerData *player, MineData *mine)
{
	// Apply the background image.
	sdl->apply_surface(0, 0, bad_background, sdl->return_screen());
	
	// Apply the header for the congratulations.
	sdl->apply_text(465, 45, "ALRIGHT!", big_header_font, sdl->return_screen());
	
	// Congratulatory text.
	sdl->apply_text(445, 90, "You managed to get", standard_font, sdl->return_screen());
	sdl->apply_text(445, 115, "the diamond but", standard_font, sdl->return_screen());
	sdl->apply_text(445, 140, "not enough money", standard_font, sdl->return_screen());
	sdl->apply_text(445, 165, "to keep Mimi happy!", standard_font, sdl->return_screen());
	sdl->apply_text(445, 190, "She turned to food", standard_font, sdl->return_screen());
	sdl->apply_text(445, 215, "out of depression.", standard_font, sdl->return_screen());
	
	// Apply the header for the statistics.
	sdl->apply_text(495, 250, "STATS", big_header_font, sdl->return_screen());
	
	// Stats to list: total gold, % of mine explored, # of turns
	sdl->apply_text(445, 300, "Total Gold:", standard_font, sdl->return_screen());
	sdl->apply_text(445, 325, "% Explored:", standard_font, sdl->return_screen());
	sdl->apply_text(445, 350, "# of Turns:", standard_font, sdl->return_screen());
	sdl->apply_text(445, 375, "Health:", standard_font, sdl->return_screen());
	
	// Stores the temporary values for strings being posted on the screen.
	std::string temp_string;
	std::stringstream temp_stringstream;	
	int temp_int = 0;
	
	temp_int = player->get_money();
	temp_stringstream << temp_int;
	temp_string = temp_stringstream.str();
	sdl->apply_text(610, 300, temp_string, standard_font, sdl->return_screen());
	temp_stringstream.str("");
	
	// Compute the amount of the mine that has been explored
	temp_int = 0;
	for(int x = 1; x < mine->get_map_x(); x++)
	{
		for(int y = 0; y < mine->get_map_y(); y++)
		{
			if(mine->get_explored(x, y))
			{
				temp_int++;
			}
		}
	}
	
	// Calculate the percentage of the mine that has been explored.
	temp_int = ((double)temp_int / 36864) * 100;
	temp_stringstream << temp_int << "%";
	temp_string = temp_stringstream.str();
	sdl->apply_text(610, 325, temp_string, standard_font, sdl->return_screen());
	temp_stringstream.str("");	
	
	// State the number of turns taken.
	temp_int = player->get_turn_number();
	temp_stringstream << temp_int;
	temp_string = temp_stringstream.str();
	sdl->apply_text(610, 350, temp_string, standard_font, sdl->return_screen());
	temp_stringstream.str("");
	
	// Display the player's ending health
	temp_int = player->get_health();
	temp_stringstream << temp_int;
	temp_string = temp_stringstream.str();
	sdl->apply_text(610, 375, temp_string, standard_font, sdl->return_screen());
	temp_stringstream.str("");

}
