/*
 bank_functions.cpp
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

#include <string>
#include <sstream>

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "sdl_functions.h"
#include "classes.h"
#include "bank_functions.h"
#include "timer.h"

void bank(PlayerData *player, SDL_Objects *sdl)
{
	// Initialize the objects within the bank screen.
	Bank_Objects bank_data;
	
	// Initialize the selection arrow.
	Selection_Arrow bank_selection(1, 6, 0, 64);
	bank_selection.set_arrow_initial(512, 0);
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
	
	// Initialize an event to track the user's input
	SDL_Event user_input;
	
	// Welcome the player to the bank.
	sdl->update_status_text("Welcome to the bank!");
	
	// Check to see if the player is eligible to have new values for minerals.
	bank_data.bank_randomize_values(player);
	
	// Apply the graphics of the bank to screen.
	bank_data.update_bank_graphics(sdl, player, &bank_selection);
	
	// Update the HUD and flip the screen.
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
		
		if(keystate[SDLK_DOWN])
		{
			if(bank_selection.move_down())
			{
				bank_data.animate_arrow_down(sdl, player, &bank_selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_UP])
		{
			if(bank_selection.move_up())
			{
				bank_data.animate_arrow_up(sdl, player, &bank_selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER])
		{
			if(bank_selection.return_vert() == 0)
			{
				// Sell all of the player's minerals.
				bank_data.bank_sell_all(player);
				bank_data.update_bank_graphics(sdl, player, &bank_selection);
			}
			else if(bank_selection.return_vert() == 1)
			{
				// Sell the player's coal.
				bank_data.bank_sell_coal(player);
				bank_data.update_bank_graphics(sdl, player, &bank_selection);
			}
			else if(bank_selection.return_vert() == 2)
			{
				// Sell the player's silver
				bank_data.bank_sell_silver(player);
				bank_data.update_bank_graphics(sdl, player, &bank_selection);
			}
			else if(bank_selection.return_vert() == 3)
			{
				// Sell the player's gold
				bank_data.bank_sell_gold(player);
				bank_data.update_bank_graphics(sdl, player, &bank_selection);
			}
			else if(bank_selection.return_vert() == 4)
			{
				// Sell the player's platinum.
				bank_data.bank_sell_platinum(player);
				bank_data.update_bank_graphics(sdl, player, &bank_selection);
			}
			else if(bank_selection.return_vert() == 5)
			{
				// Leave the bank.
				sdl->update_status_text("Thanks for banking with us!");
				sdl->display_hud(player);
				SDL_Delay(sdl->ENTER_WAIT);
				exit = true;
			}
			
			update_screen = true;
		}
		else if(keystate[SDLK_ESCAPE] || keystate[SDLK_BACKSPACE])
		{
			SDL_Delay(sdl->ENTER_WAIT);
			sdl->update_status_text("Thanks for banking with us!");
			sdl->display_hud(player);
			exit = true;
		}
			
		// Apply the graphics on screen and update them.
		if(update_screen)
		{
			bank_data.update_bank_graphics(sdl, player, &bank_selection);	
			SDL_UpdateRect(sdl->return_screen(), 500, 0, 70, 380);
			sdl->display_hud(player);
			SDL_Flip(sdl->return_screen());
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}
}

Bank_Objects::Bank_Objects()
{
	bank_graphic = IMG_Load("./Graphics/bank/bank_screen.png");
	bank_arrow_graphic = IMG_Load("./Graphics/bank/arrow.png");
	
	sell_coal_graphic = IMG_Load("./Graphics/bank/sell_coal.png");
	sell_silver_graphic = IMG_Load("./Graphics/bank/sell_silver.png");
	sell_gold_graphic = IMG_Load("./Graphics/bank/sell_gold.png");
	sell_platinum_graphic = IMG_Load("./Graphics/bank/sell_platinum.png");
	sell_all_graphic = IMG_Load("./Graphics/bank/sell_all.png");
	exit_graphic = IMG_Load("./Graphics/bank/exit_bank.png");
	
	platinum_graphic = IMG_Load("./Graphics/bank/platinum.png");
	gold_graphic = IMG_Load("./Graphics/bank/gold.png");
	silver_graphic = IMG_Load("./Graphics/bank/silver.png");
	coal_graphic = IMG_Load("./Graphics/bank/coal.png");
	
	header_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
	display_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 20);
}

Bank_Objects::~Bank_Objects()
{
	SDL_FreeSurface(bank_graphic);
	SDL_FreeSurface(bank_arrow_graphic);

	SDL_FreeSurface(sell_coal_graphic);
	SDL_FreeSurface(sell_silver_graphic);
	SDL_FreeSurface(sell_gold_graphic);
	SDL_FreeSurface(sell_platinum_graphic);
	SDL_FreeSurface(sell_all_graphic);
	SDL_FreeSurface(exit_graphic);	
	
	SDL_FreeSurface(platinum_graphic);
	SDL_FreeSurface(gold_graphic);
	SDL_FreeSurface(silver_graphic);
	SDL_FreeSurface(coal_graphic);
	
	TTF_CloseFont(header_font);
	TTF_CloseFont(display_font);
}

void Bank_Objects::bank_sell_all(PlayerData *player)
{
	bank_sell_platinum(player);
	bank_sell_gold(player);
	bank_sell_silver(player);
	bank_sell_coal(player);
}

void Bank_Objects::bank_sell_platinum(PlayerData *player)
{
	player->change_money(player->get_platinum() * player->get_platinum_value());
	player->change_platinum(player->get_platinum() * -1);
}
		
void Bank_Objects::bank_sell_gold(PlayerData *player)
{
	player->change_money(player->get_gold() * player->get_gold_value());
	player->change_gold(player->get_gold() * -1);
}

void Bank_Objects::bank_sell_silver(PlayerData *player)
{
	player->change_money(player->get_silver() * player->get_silver_value());
	player->change_silver(player->get_silver() * -1);
}
	
void Bank_Objects::bank_sell_coal(PlayerData *player)
{
	player->change_money(player->get_coal() * player->get_coal_value());
	player->change_coal(player->get_coal() * -1);
}

void Bank_Objects::bank_randomize_values(PlayerData *player)
{
	if((player->get_turn_number() - player->get_previous_turn_number()) >= 10)
	{
		player->randomize_coal_value();
		player->randomize_silver_value();
		player->randomize_gold_value();
		player->randomize_platinum_value();
		
		player->set_previous_turn_number();
	}	
}

void Bank_Objects::update_bank_graphics(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *bank_selection)
{	
    // Call the general bank graphics.
    display_bank_information(sdl, player);
    
	// Display the current location of the arrow.
	sdl->apply_surface(bank_selection->return_arrow_x(), bank_selection->return_arrow_y(),
						bank_arrow_graphic, sdl->return_screen());
    
}

// Update the main items on the screen. Allows for animation.
void Bank_Objects::display_bank_information(SDL_Objects *sdl, PlayerData *player)
{
    
	// Display the background image.
	sdl->apply_surface(0, 0, bank_graphic, sdl->return_screen());    
    
	// Display the header graphic
	sdl->apply_text(90, 10, "TODAY'S FINE PRICES:", header_font, sdl->return_screen());       
    
	// Stores the temporary values for strings being posted on the screen.
	std::string temp_string;
	std::stringstream temp_stringstream;
	int temp_money_value = 0;
	
	// Display items on the sidebar
	sdl->apply_surface(576, 0, sell_all_graphic, sdl->return_screen());
	sdl->apply_surface(576, 64, sell_coal_graphic, sdl->return_screen());
	sdl->apply_surface(576, 128, sell_silver_graphic, sdl->return_screen());
	sdl->apply_surface(576, 192, sell_gold_graphic, sdl->return_screen());
	sdl->apply_surface(576, 256, sell_platinum_graphic, sdl->return_screen());
	sdl->apply_surface(576, 320, exit_graphic, sdl->return_screen());   
    
	// Apply the listings of current prices and graphics.
	sdl->apply_surface(35, 60, coal_graphic, sdl->return_screen());
    temp_stringstream << "Coal's value: " << player->get_coal_value() << " x "
    << player->get_coal();
    temp_string = temp_stringstream.str();
    sdl->apply_text(100, 80, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
    
    temp_money_value = player->get_coal_value() * player->get_coal();
    temp_stringstream << temp_money_value;
    temp_string = temp_stringstream.str();
    sdl->apply_text(440, 80, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
    
	sdl->apply_surface(35, 125, silver_graphic, sdl->return_screen());
    temp_stringstream << "Silver's value: " << player->get_silver_value() << " x "
    << player->get_silver();
    temp_string = temp_stringstream.str();
    sdl->apply_text(100, 145, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
    
    temp_money_value = player->get_silver_value() * player->get_silver();
    temp_stringstream << temp_money_value;
    temp_string = temp_stringstream.str();
    sdl->apply_text(440, 145, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
    
	sdl->apply_surface(35, 195, gold_graphic, sdl->return_screen());
    temp_stringstream << "Gold's value: " << player->get_gold_value() << " x "
    << player->get_gold();
    temp_string = temp_stringstream.str();
    sdl->apply_text(100, 210, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
    
    temp_money_value = player->get_gold_value() * player->get_gold();
    temp_stringstream << temp_money_value;
    temp_string = temp_stringstream.str();
    sdl->apply_text(440, 210, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
    
	sdl->apply_surface(35, 255, platinum_graphic, sdl->return_screen());
    temp_stringstream << "Platinum's value: " << player->get_platinum_value() << " x "
    << player->get_platinum();
    temp_string = temp_stringstream.str();
    sdl->apply_text(100, 275, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
    
    temp_money_value = player->get_platinum_value() * player->get_platinum();
    temp_stringstream << temp_money_value;
    temp_string = temp_stringstream.str();
    sdl->apply_text(440, 275, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
    
    sdl->apply_text(150, 340, "Value of all minerals: ", display_font, sdl->return_screen());
	
    temp_money_value = (player->get_coal_value() * player->get_coal()) +
    (player->get_silver_value() * player->get_silver()) +
    (player->get_gold_value() * player->get_gold()) +
    (player->get_platinum_value() * player->get_platinum());
    temp_stringstream << temp_money_value;
    temp_string = temp_stringstream.str();
    sdl->apply_text(440, 340, temp_string, display_font, sdl->return_screen());
    temp_stringstream.str("");
}

// Animate movement of the arrow.
void Bank_Objects::animate_arrow_down(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() - 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp + 16;
        display_bank_information(sdl, player);
		sdl->apply_surface(selection->return_arrow_x(), temp, bank_arrow_graphic, sdl->return_screen());
		SDL_UpdateRect(sdl->return_screen(), 500, 0, 70, 380);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}

void Bank_Objects::animate_arrow_up(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() + 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp - 16;
        display_bank_information(sdl, player);
		sdl->apply_surface(selection->return_arrow_x(), temp, bank_arrow_graphic, sdl->return_screen());
		SDL_UpdateRect(sdl->return_screen(), 500, 0, 70, 380);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}
