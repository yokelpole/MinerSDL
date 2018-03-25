/*
 high_scores.cpp
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

// Used for access to string libraries, etc.
#include <string.h>
#include <sstream>

// Used for the writing/accessing of files on the drive.
#include <iostream>
#include <fstream>

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "high_scores.h"
#include "sdl_functions.h"
#include "classes.h"

// Initial function to load up the high scores and to display them.
void display_high_scores(SDL_Objects *sdl, PlayerData *player, bool high_score_entry)
{
	High_Score_Objects high_score_screen;

	high_score_screen.load_high_scores();				// Load the high scores from the file.	
	
	if(high_score_entry)
	{
		if(high_score_screen.check_high_score(player))		// Check to see if the player's new score fits into the high scores.
		{
			high_score_screen.name_entry_for_high_score(sdl, player);		
			high_score_screen.rearrange_high_scores();		// Rearrange the high scores if a new one has been added.
		}
	}
		
	high_score_screen.update_high_score_graphics(sdl);		// Update the graphics.

	high_score_screen.wait_for_keypress(sdl);
	
	high_score_screen.write_high_scores();
}

High_Score_Objects::High_Score_Objects()
{
	background = IMG_Load("./Graphics/high_score/background.png");
	name_entry = IMG_Load("./Graphics/high_score/name_entry.png");
	diamond_graphic = IMG_Load("./Graphics/high_score/diamond.png");
	mimi_happy_graphic = IMG_Load("./Graphics/high_score/mimi_happy.png");
	mimi_sad_graphic = IMG_Load("./Graphics/high_score/mimi_sad.png");
	headstone_graphic = IMG_Load("./Graphics/high_score/headstone.png");
	
	header_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 36);
	standard_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
	
	high_score_name = "";
	
	// Set all variables to nothing.
	for(int x = 0; x <= 4; x++)
	{
		player_name[x] = "blank";			
		player_money[x] = 0;
		had_diamond[x] = false;
		player_dead[x] = false;
	}
}

High_Score_Objects::~High_Score_Objects()
{
	SDL_FreeSurface(background);
	SDL_FreeSurface(diamond_graphic);
	SDL_FreeSurface(mimi_happy_graphic);
	SDL_FreeSurface(mimi_sad_graphic);
	SDL_FreeSurface(headstone_graphic);
	
	TTF_CloseFont(header_font);
	TTF_CloseFont(standard_font);
}

void High_Score_Objects::update_high_score_graphics(SDL_Objects *sdl)
{
	std::string temp_string;				// Used as a buffer for numerical values (int)
	std::stringstream temp_stringstream;	// Used to convert the int to health string
	
	sdl->apply_surface(0, 0, background, sdl->return_screen());
	sdl->apply_colored_text(245, 10, 198, 15, 15, "HIGH SCORES", header_font, sdl->return_screen());
	
	// Display the players' scores, etc.
	for(int x = 0; x <= 4; x++)
	{
		temp_stringstream << player_money[x];
		temp_string = temp_stringstream.str();
		
		sdl->apply_text(50, 75 + (x * 80), temp_string.c_str(), standard_font, sdl->return_screen());
		sdl->apply_text(200, 75 + (x * 80), player_name[x].c_str(), standard_font, sdl->return_screen());
		
		if(had_diamond[x])
		{
			sdl->apply_surface(705, 60 + (x * 80), diamond_graphic, sdl->return_screen());
		}
		
		if(player_dead[x])
		{
			sdl->apply_surface(655, 60 + (x * 80), headstone_graphic, sdl->return_screen());
		}
		
		if(player_money[x] < 5000)
		{
			sdl->apply_surface(605, 60 + (x * 80), mimi_sad_graphic, sdl->return_screen());
		}
		else if(player_money[x] >= 5000)
		{
			sdl->apply_surface(605, 60 + (x * 80), mimi_happy_graphic, sdl->return_screen());
		}

		temp_stringstream.str("");
	}
		
	
	SDL_Flip(sdl->return_screen());
}

void High_Score_Objects::name_entry_for_high_score(SDL_Objects *sdl, PlayerData *player)
{
	// To catch the user's input.
	SDL_Event user_input;
	
	// Timer to allow for keyboard action tracking
	Timer keyboard_timer;	
	
	// Testing stuff...
	bool exit = false;
	bool update_screen = false;
	
	// Clear the string where we're putting the Player's name
	player_name[4] = "";
	
	// Update the graphics initially
	sdl->apply_surface(0, 0, background, sdl->return_screen());		
	sdl->apply_surface(64, 140, name_entry, sdl->return_screen());
	sdl->apply_text(160, 170, "You got a high score!", header_font, sdl->return_screen());
	sdl->apply_text(180, 210, "Please enter your name!", standard_font, sdl->return_screen());
	SDL_Flip(sdl->return_screen());
	
	// Enable unicode for text entry and key repeating.
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(250, 250);
	
	// Wait for the user's input.	
	while(!exit && !sdl->return_quit_to_menu())
	{	
		update_screen = false;
		Uint8 *keystate = SDL_GetKeyState(NULL);
			
        //While there's events to handle
        while( SDL_PollEvent( &user_input ) )
        {
			// Quit if the user chooses to close the window.
			if(user_input.type == SDL_QUIT)
			{
				sdl->set_quitSDL();
				exit = true;
			}
			
			if(user_input.type == SDL_KEYDOWN)
			{
				// if key is ASCII, accept it as character input
				if(keystate[ SDLK_RETURN ] || keystate[ SDLK_KP_ENTER])
				{
					exit = true;
				}
				else if(keystate[ SDLK_BACKSPACE ])
				{
						if(player_name[4].size() > 0)
						{
							player_name[4].erase(player_name[4].size() - 1, 1);
						}
				}
				
				if(player_name[4].size() < 14)
				{
					//If the key is a number
					if( ( user_input.key.keysym.unicode >= (Uint16)'0' ) && ( user_input.key.keysym.unicode <= (Uint16)'9' ) )
					{
						//Append the character
						player_name[4] += (char)user_input.key.keysym.unicode;
					}
					//If the key is a uppercase letter
					else if( ( user_input.key.keysym.unicode >= (Uint16)'A' ) && ( user_input.key.keysym.unicode <= (Uint16)'Z' ) )
					{
						//Append the character
						player_name[4] += (char)user_input.key.keysym.unicode;
					}
					//If the key is a lowercase letter
					else if( ( user_input.key.keysym.unicode >= (Uint16)'a' ) && ( user_input.key.keysym.unicode <= (Uint16)'z' ) )
					{
						//Append the character
						player_name[4] += (char)user_input.key.keysym.unicode;
					}		
					
					update_screen = true;						
				}
			}	    
        }

		if(update_screen)
		{
			sdl->apply_surface(0, 0, background, sdl->return_screen());		
			sdl->apply_surface(64, 140, name_entry, sdl->return_screen());
			sdl->apply_text(160, 170, "You got a high score!", header_font, sdl->return_screen());
			sdl->apply_text(180, 210, "Please enter your name!", standard_font, sdl->return_screen());
			sdl->apply_text(100, 260, player_name[4].c_str(), header_font, sdl->return_screen());
			
			SDL_Flip(sdl->return_screen());
			update_screen = false;
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}
	
	// Prevent the player from having no name, otherwise the scoreboard gets messed up.
	if(player_name[4] == "")
	{
		player_name[4] = "Unnamed";
	}
	
	SDL_EnableUNICODE(0);
	SDL_EnableKeyRepeat(NULL, NULL);
}


// Save the information to the high score file.
void High_Score_Objects::write_high_scores()
{
	std::ofstream high_score_out("high_scores", std::ios::binary);
	
	for(int x = 0; x <= 4; x++)
	{
		high_score_out << player_name[x] << std::endl;	
		high_score_out << player_money[x] << std::endl;
		high_score_out << had_diamond[x] << std::endl;
		high_score_out << player_dead[x] << std::endl;
	}
	
	
	high_score_out.close();
}

// Load the information from the high score file
void High_Score_Objects::load_high_scores()
{
	std::ifstream high_score_in("high_scores", std::ios::binary);
	
	for(int x = 0; x <= 4; x++)
	{
		high_score_in >> player_name[x];
		high_score_in >> player_money[x];
		high_score_in >> had_diamond[x];
		high_score_in >> player_dead[x];
	}
	
	high_score_in.close();
}

// Check to see if a high score can be entered into the current set.
bool High_Score_Objects::check_high_score(PlayerData *player)
{
	for(int x = 0; x <= 4; x++)
	{
		// If it is a high score, then move the data into the lowest
		// slot. Another function will rearrange.
		if(player->get_money() > player_money[x])
		{
			player_money[4] = player->get_money();
			had_diamond[4] = player->get_has_diamond();
			
			if(player->get_health() <= 0)
			{
				player_dead[4] = true;
			}
			else
			{
				player_dead[4] = false;
			}
			
			return true;
		}
	}
	
	return false;
}
		
// Rearrange the high scores once one has been entered.
void High_Score_Objects::rearrange_high_scores()
{
	int player_money_temp;
	std::string player_name_temp;
	bool had_diamond_temp;
	bool player_dead_temp;
	
	bool in_order = false;
	
	while(!in_order)
	{
		// Quit if all of the scores are in proper order...
		if(player_money[0] >= player_money[1]
		&& player_money[1] >= player_money[2]
		&& player_money[2] >= player_money[3]
		&& player_money[3] >= player_money[4])
		{
			in_order = true;
		}
		// ... if not, then rearrange till they are.
		else
		{
			for(int y = 0; y <= 4; y++)
			{
				for(int x = 0; x <= 4; x++)
				{
					if(player_money[y] < player_money[x]
						&& y < x)
					{
						player_money_temp = player_money[y];
						player_name_temp = player_name[y];
						had_diamond_temp = had_diamond[y];
						player_dead_temp = player_dead[y];
						
						player_money[y] = player_money[x];
						player_name[y] = player_name[x];
						had_diamond[y] = had_diamond[x];
						player_dead[y] = player_dead[x];
						
						player_money[x] = player_money_temp;
						player_name[x] = player_name_temp;
						had_diamond[x] = had_diamond_temp;
						player_dead[x] = player_dead_temp;
					}
				}
			}
		}
	}
}

// Wait for a user keypress to exit the high score screen.
void High_Score_Objects::wait_for_keypress(SDL_Objects *sdl)
{
	// Function waits for player to press enter key before progressing.
	bool stop_loop = false;
	
	SDL_Event user_input;
	Timer keyboard_timer;
	
	// Stop a keypress from earlier being carried over.
	SDL_Delay(sdl->KEYPRESS_WAIT);
	
	while(!stop_loop)
	{		
		while(SDL_PollEvent(&user_input))
		{	
			// Quit if the user chooses to close the window.
			if(user_input.type == SDL_QUIT)
			{
				sdl->quit_sdl();
				return;
			}
			else if(user_input.type == SDL_KEYDOWN)
			{
				stop_loop = true;
			}
			else if(user_input.type == SDL_KEYUP)
			{
				keyboard_timer.stop_timer();
			}
		}
			
		SDL_Delay(sdl->SDL_WAIT);
	}
	
	SDL_Delay(sdl->ENTER_WAIT);
}
