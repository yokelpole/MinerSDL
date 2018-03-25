/*
 hospital_functions.cpp
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

#include <string>
#include <sstream>

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "sdl_functions.h"
#include "classes.h"
#include "hospital_functions.h"
#include "popup_menu.h"
#include "timer.h"

void hospital(PlayerData *player, SDL_Objects *sdl)
{
	// Initialize object to store hospital's data in.
	Hospital_Objects hospital_data;

	// To catch the user's input.
	SDL_Event user_input;
	
	// Initialize the selection arrow.
	Selection_Arrow hospital_selection(1, 4, 0, 64);
	hospital_selection.set_arrow_initial(512, 0);
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
	
	sdl->update_status_text("Welcome to the hospital!");
	
	// Update the screen for the first time.
	hospital_data.update_hospital_graphics(sdl, player, &hospital_selection);
	sdl->display_hud(player);
	SDL_Flip(sdl->return_screen());
	
	// Wait for the user's input.
	while(!exit)
	{
		update_screen = false;
		Uint8 *keystate = SDL_GetKeyState(NULL);	// Captures keyboard input.
		
		while(SDL_PollEvent(&user_input))
		{	
			// Quit if the user chooses to close the window.
			if(user_input.type == SDL_QUIT)
			{
				sdl->set_quitSDL();
				exit = true;
			}
		}
		
		// Respond to the user's key presses
		if(keystate[SDLK_DOWN])
		{
			if(hospital_selection.move_down())
			{
				hospital_data.animate_arrow_down(sdl, &hospital_selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_UP])
		{
			if(hospital_selection.move_up())
			{
				hospital_data.animate_arrow_up(sdl, &hospital_selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER])
		{
			if(hospital_selection.return_vert() == 0)
			{
				// Allow the player to stay for one day/night.
				hospital_data.stay_one_day(player);
				SDL_Delay(sdl->KEYPRESS_WAIT);
				update_screen = true;
			}
			else if(hospital_selection.return_vert() == 1)
			{				
				SDL_Delay(sdl->KEYPRESS_WAIT);

				// Allow the player to stay until health is refilled.
				// Run a check to see if doing this will break the bank.
				if(player->get_money() - ((100 - player->get_health()) * 10) <= 0)
				{
					if(display_confirm_spend(sdl))
					{
						hospital_data.full_heal(player);
					}
				}
				else
				{
					hospital_data.full_heal(player);
				}
				
				update_screen = true;
			}
			else if(hospital_selection.return_vert() == 2)
			{
				// Allow the player to purchase insurance.
				if(player->get_money() >= 250
					&& (player->get_insurance_turn_number() + 25) < player->get_turn_number())
				{
					hospital_data.insurance(player);
				}
				else if(player->get_money() < 250)
				{
					sdl->update_status_text("You can't afford insurance!");
				}
				else if(player->get_insurance_turn_number() == player->get_turn_number())
				{
					sdl->update_status_text("You already have insurance for max turns!");
				}

				SDL_Delay(sdl->KEYPRESS_WAIT);			
				update_screen = true;
			}
			else if(hospital_selection.return_vert() == 3)
			{
				// Return to the town screen.
				sdl->update_status_text("A good rinse sterilizes, right?");
				sdl->display_hud(player);
				SDL_Delay(sdl->ENTER_WAIT);
				exit = true;
			}
		}
		else if(keystate[SDLK_ESCAPE] || keystate[SDLK_BACKSPACE])
		{
			SDL_Delay(sdl->ENTER_WAIT);	
			sdl->update_status_text("A good rinse sterilizes, right?");
			sdl->display_hud(player);
			exit = true;
		}
		
		if(update_screen)
		{	
			// Apply the graphics on screen and update them.
			hospital_data.update_hospital_graphics(sdl, player, &hospital_selection);
			sdl->display_hud(player);
			SDL_Flip(sdl->return_screen());
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}
}

Hospital_Objects::Hospital_Objects()
{
	hospital_graphic = IMG_Load("./Graphics/hospital/hospital.png");
	hospital_arrow_graphic = IMG_Load("./Graphics/hospital/arrow.png");
	one_day_button = IMG_Load("./Graphics/hospital/one_day.png");
	full_heal_button = IMG_Load("./Graphics/hospital/refill_health.png");
	insurance_button = IMG_Load("./Graphics/hospital/insurance.png");
	exit_button = IMG_Load("./Graphics/hospital/exit_hospital.png");
	
	header_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
	display_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 20);
}

Hospital_Objects::~Hospital_Objects()
{
	SDL_FreeSurface(hospital_graphic);
	SDL_FreeSurface(hospital_arrow_graphic);
	SDL_FreeSurface(one_day_button);
	SDL_FreeSurface(full_heal_button);
	SDL_FreeSurface(insurance_button);
	SDL_FreeSurface(exit_button);
	
	TTF_CloseFont(header_font);
	TTF_CloseFont(display_font);
}

void Hospital_Objects::update_hospital_graphics(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *hospital_selection)
{		
	// Stores the temporary values for strings being posted on the screen.
	std::string temp_string;
	std::stringstream temp_stringstream;
	
	// Display the background image.
	sdl->apply_surface(0, 0, hospital_graphic, sdl->return_screen());
	
	// Display the buttons on the sidebar.
	sdl->apply_surface(576, 0, one_day_button, sdl->return_screen());
	sdl->apply_surface(576, 64, full_heal_button, sdl->return_screen());
	sdl->apply_surface(576, 128, insurance_button, sdl->return_screen());
	sdl->apply_surface(576, 192, exit_button, sdl->return_screen());

	// Update the location of the selection arrow.
	sdl->apply_surface(hospital_selection->return_arrow_x(), hospital_selection->return_arrow_y(),
						hospital_arrow_graphic, sdl->return_screen());

	// Update the header for the hospital.
	sdl->apply_text(75, 10, "RUSTY'S RESTORATION", header_font, sdl->return_screen());
	
	// Update the text in the information window depending on where the
	// player's cursor currently is.
	if(hospital_selection->return_vert() == 0)
	{
		// If player has selected 'one day'
		sdl->apply_text(10, 75, "Stay for one day.", display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "If all goes well, you'll get 10 health back.", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "Don't mind the bedbugs, they don't bite...", display_font, sdl->return_screen());
		sdl->apply_text(10, 175, "Hard.", display_font, sdl->return_screen());
		sdl->apply_text(115, 300, "COST: $100/DAY", header_font, sdl->return_screen());
	}
	else if(hospital_selection->return_vert() == 1)
	{
		// If player has selected 'full heal'
		sdl->apply_text(10, 75, "Refill your health entirely." , display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "The cost of fully healing depends on how", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "hurt you are. You receive one point of", display_font, sdl->return_screen());
		sdl->apply_text(10, 175, "health for every ten dollars spent.", display_font, sdl->return_screen());
		sdl->apply_text(10, 225, "There isn't a bulk discount.", display_font, sdl->return_screen());
		sdl->apply_text(10, 250, "This isn't a grocery store.", display_font, sdl->return_screen());
	
		temp_stringstream << "CURRENT COST: $" << ((100 - player->get_health()) * 10);
		temp_string = temp_stringstream.str();
		sdl->apply_text(80, 300, temp_string, header_font, sdl->return_screen());
		temp_stringstream.str("");
		temp_string = "";
	}
	else if(hospital_selection->return_vert() == 2)
	{
		// If player has selected 'insurance'
		sdl->apply_text(10, 75, "Insurance can keep you alive.", display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "If you become fatally wounded within", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "the next 50 turns, you are able to", display_font, sdl->return_screen());
		sdl->apply_text(10, 175, "instantly regenerate 35 health.", display_font, sdl->return_screen());
//		sdl->apply_text(10, 200, "", display_font, sdl->return_screen());
		sdl->apply_text(10, 250, "Good luck returning to the surface!", display_font, sdl->return_screen());
		sdl->apply_text(150, 300, "COST: $250", header_font, sdl->return_screen());
	}
	else if(hospital_selection->return_vert() == 3)
	{
		// If player has selected to exit
		sdl->apply_text(10, 75, "Think you feel better?", display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "Then get the hell outta here!", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "Paying customers only.", display_font, sdl->return_screen());
	}
}

void Hospital_Objects::stay_one_day(PlayerData *player)
{
	if(player->get_health() <= 100 && player->get_money() >= 10)
	{
		player->change_health(10);
		player->change_money(-100);
	}
}

void Hospital_Objects::full_heal(PlayerData *player)
{
	while(player->get_health() < 100 && player->get_money() >= 10)
	{
		player->change_health(1);
		player->change_money(-10);
	}
}

void Hospital_Objects::insurance(PlayerData *player)
{
	player->change_has_insurance(true);
	player->set_insurance_turn_number();
	player->change_money(-250);
}

// Animate movement of the arrow.
void Hospital_Objects::animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() - 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp + 16;
        
        // Display the background image.
        sdl->apply_surface(0, 0, hospital_graphic, sdl->return_screen());
        
        // Display the buttons on the sidebar.
        sdl->apply_surface(576, 0, one_day_button, sdl->return_screen());
        sdl->apply_surface(576, 64, full_heal_button, sdl->return_screen());
        sdl->apply_surface(576, 128, insurance_button, sdl->return_screen());
        sdl->apply_surface(576, 192, exit_button, sdl->return_screen());
        
        // Update the location of the selection arrow.
        sdl->apply_surface(selection->return_arrow_x(), temp,
                           hospital_arrow_graphic, sdl->return_screen());
        
        // Update the header for the hospital.
        sdl->apply_text(75, 10, "RUSTY'S RESTORATION", header_font, sdl->return_screen());
        
		SDL_UpdateRect(sdl->return_screen(), 505, 0, 70, 380);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}

void Hospital_Objects::animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() + 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp - 16;
        
        // Display the background image.
        sdl->apply_surface(0, 0, hospital_graphic, sdl->return_screen());
        
        // Display the buttons on the sidebar.
        sdl->apply_surface(576, 0, one_day_button, sdl->return_screen());
        sdl->apply_surface(576, 64, full_heal_button, sdl->return_screen());
        sdl->apply_surface(576, 128, insurance_button, sdl->return_screen());
        sdl->apply_surface(576, 192, exit_button, sdl->return_screen());
        
        // Update the location of the selection arrow.
        sdl->apply_surface(selection->return_arrow_x(), temp,
                           hospital_arrow_graphic, sdl->return_screen());
        
        // Update the header for the hospital.
        sdl->apply_text(75, 10, "RUSTY'S RESTORATION", header_font, sdl->return_screen());
		
        SDL_UpdateRect(sdl->return_screen(), 505, 0, 70, 380);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}
