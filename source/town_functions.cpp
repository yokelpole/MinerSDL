/*
 town_functions.cpp
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
 Functions that deal with the different areas of the town.
  
 The player is able to access the main menu, bank, hospital, store,
 tavern, or the mine. 

 The main starting point for a new/loaded game.
*/

// General SDL includes.
#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

// General program includes
#include "sdl_functions.h"
#include "town_functions.h"
#include "classes.h"
#include "timer.h"

// Files for the different stores and mine.
#include "bank_functions.h"
#include "hospital_functions.h"
#include "store_functions.h"
#include "mine.h"
#include "tavern.h"
#include "popup_menu.h"

// Loads and displays the screen for the main town.
void main_town(PlayerData *player, MineData *mine, SDL_Objects *sdl)
{
	// Initialize the objects within the town.
	Town_Objects town;
		
	// Initialize the selection arrow
	Selection_Arrow selection(1, 5, 0, 64);		// Initializes the arrow object with six places to go up/down.
	selection.set_arrow_initial(512, 0);
	
	// Initialize an event to track the user's input
	SDL_Event user_input;
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
	
	// Welcome the player to town.
	sdl->update_status_text("Welcome to town!");
	
	// Display the HUD on the screen.
	sdl->display_hud(player);	
	
	// Apply the graphics on screen and update them.
	town.update_town_graphics(sdl, &selection);	
	SDL_Flip(sdl->return_screen());
	
	// Main loop for the town
	while(!exit && !sdl->return_quitSDL() && !sdl->return_quit_to_menu())
	{
		update_screen = false;
		Uint8 *keystate = SDL_GetKeyState(NULL);	// Captures keyboard input.
		
		while(SDL_PollEvent( &user_input ))
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
			if(selection.move_down())
			{
				town.animate_arrow_down(sdl, &selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_UP])
		{
			if(selection.move_up())
			{
				town.animate_arrow_up(sdl, &selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER])
		{
			if(selection.return_vert() == 0)
			{
				// Head to the bank.
				SDL_Delay(sdl->ENTER_WAIT);
				bank(player, sdl);
				
				// After returning from function, check to see if player has
				// decided to quit the game.
				if(sdl->return_quitSDL() == true)
				{
					return;
				}
					
				town.update_town_graphics(sdl, &selection);	
				SDL_Flip(sdl->return_screen());
			}
			else if(selection.return_vert() == 1)
			{
				// Head to the bar.
				SDL_Delay(sdl->ENTER_WAIT);
				
				tavern(player, mine, sdl);
				
				town.update_town_graphics(sdl, &selection);	
				
				if(!sdl->return_quit_to_menu())
				{
					SDL_Flip(sdl->return_screen());
				}
			}
			else if(selection.return_vert() == 2)
			{
				// Head to the hospital.
				SDL_Delay(sdl->ENTER_WAIT);
				hospital(player, sdl);
				
				town.update_town_graphics(sdl, &selection);	
				SDL_Flip(sdl->return_screen());
			}
			else if(selection.return_vert() == 3)
			{
				// Head to the general store.
				SDL_Delay(sdl->ENTER_WAIT);
				store(player, sdl);
				
				town.update_town_graphics(sdl, &selection);	
				SDL_Flip(sdl->return_screen());
			}
			else if(selection.return_vert() == 4)
			{
				// Head to the mine
				mine_function(player, sdl, mine);
				
				// Only refresh the screen if the player isn't quitting.
				if(!sdl->return_quitSDL() && !sdl->return_quit_to_menu())
				{ 
					town.update_town_graphics(sdl, &selection);
					SDL_Flip(sdl->return_screen());
				}
			}
			
			if(!sdl->return_quitSDL() && !sdl->return_quit_to_menu())
			{
				SDL_Delay(sdl->ENTER_WAIT);
			}
		}
		else if(keystate[SDLK_ESCAPE] || keystate[SDLK_BACKSPACE])
		{
			// Popup the menu.
			SDL_Delay(sdl->ENTER_WAIT);
			display_popup_menu(sdl, mine, player);
			
			town.update_town_graphics(sdl, &selection);
			SDL_Flip(sdl->return_screen());
		}
				
		if(update_screen)
		{	
			// Apply the graphics on screen and update them.
			town.update_town_graphics(sdl, &selection);	
			SDL_UpdateRect(sdl->return_screen(), 500, 0, 70, 380);
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}
}


Town_Objects::Town_Objects()
{
	town_graphic = IMG_Load("./Graphics/town/town.png");
	
	bank_graphic = IMG_Load("./Graphics/town/bank_button.png");
	tavern_graphic = IMG_Load("./Graphics/town/bar_button.png");
	hospital_graphic = IMG_Load("./Graphics/town/hospital_button.png");
	store_graphic = IMG_Load("./Graphics/town/store_button.png");
	mine_graphic = IMG_Load("./Graphics/town/mine_button.png");
	
	arrow_graphic = IMG_Load("./Graphics/town/arrow.png");

	display_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
}

Town_Objects::~Town_Objects()
{
	SDL_FreeSurface(town_graphic);

	SDL_FreeSurface(bank_graphic);
	SDL_FreeSurface(tavern_graphic);
	SDL_FreeSurface(hospital_graphic);
	SDL_FreeSurface(store_graphic);
	SDL_FreeSurface(mine_graphic);

	SDL_FreeSurface(arrow_graphic);
}

// Update the graphics for the town.
void Town_Objects::update_town_graphics(SDL_Objects *sdl, Selection_Arrow *selection)
{
	sdl->apply_surface(0, 0, town_graphic, sdl->return_screen());
	sdl->apply_surface(selection->return_arrow_x(), selection->return_arrow_y(), arrow_graphic, sdl->return_screen());

	// Display items on the sidebar
	sdl->apply_surface(576, 0, bank_graphic, sdl->return_screen());
	sdl->apply_surface(576, 64, tavern_graphic, sdl->return_screen());
	sdl->apply_surface(576, 128, hospital_graphic, sdl->return_screen());
	sdl->apply_surface(576, 192, store_graphic, sdl->return_screen());
	sdl->apply_surface(576, 256, mine_graphic, sdl->return_screen());
}

// Animate movement of the arrow.
void Town_Objects::animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() - 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp + 16;
		sdl->apply_surface(0, 0, town_graphic, sdl->return_screen());
        
        // Display items on the sidebar
        sdl->apply_surface(576, 0, bank_graphic, sdl->return_screen());
        sdl->apply_surface(576, 64, tavern_graphic, sdl->return_screen());
        sdl->apply_surface(576, 128, hospital_graphic, sdl->return_screen());
        sdl->apply_surface(576, 192, store_graphic, sdl->return_screen());
        sdl->apply_surface(576, 256, mine_graphic, sdl->return_screen());
        
		sdl->apply_surface(selection->return_arrow_x(), temp, arrow_graphic, sdl->return_screen());
		SDL_UpdateRect(sdl->return_screen(), 500, 0, 70, 380);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}

void Town_Objects::animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() + 64;
	
	while(temp != selection->return_arrow_y())
	{
		temp = temp - 16;
		sdl->apply_surface(0, 0, town_graphic, sdl->return_screen());
        
        // Display items on the sidebar
        sdl->apply_surface(576, 0, bank_graphic, sdl->return_screen());
        sdl->apply_surface(576, 64, tavern_graphic, sdl->return_screen());
        sdl->apply_surface(576, 128, hospital_graphic, sdl->return_screen());
        sdl->apply_surface(576, 192, store_graphic, sdl->return_screen());
        sdl->apply_surface(576, 256, mine_graphic, sdl->return_screen());
        
		sdl->apply_surface(selection->return_arrow_x(), temp, arrow_graphic, sdl->return_screen());
		SDL_UpdateRect(sdl->return_screen(), 500, 0, 70, 380);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}
