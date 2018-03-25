/*
 startup_screen.cpp
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

// General SDL includes.
#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"

// General program includes
#include "sdl_functions.h"
#include "classes.h"
#include "timer.h"
#include "startup_screen.h"
#include "save_load.h"
#include "high_scores.h"
#include "instructions.h"

void startup_screen(PlayerData *player, MineData *mine, SDL_Objects *sdl)
{
	// Create the store object to store data in.
	Start_Screen screen_data;
	
	// To catch the user's input.
	SDL_Event user_input;
	
	// Create the selection box.
	Selection_Arrow selection(1, 5, 0, 64);
	selection.set_arrow_initial(416, 136);
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
	
	// Initially show the screen.
	screen_data.update_start_screen(sdl, &selection);
	SDL_Flip(sdl->return_screen());
	
	// Main loop for the start screen
	while(!exit && sdl->return_quit_to_menu() == true)
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
				screen_data.animate_arrow_down(sdl, &selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_UP])
		{
			if(selection.move_up())
			{
				screen_data.animate_arrow_up(sdl, &selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_ESCAPE])
		{
			sdl->set_quitSDL();
			exit = true;
		}
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER])
		{
			if(selection.return_vert() == 0)
			{
				// Start a new game.				
				sdl->set_quit_to_menu(false);
				exit = true;
				
				SDL_Delay(sdl->ENTER_WAIT);
			}
			else if(selection.return_vert() == 1)
			{
				// Load a previously saved game.
				load_game(mine, player);
				
				sdl->set_quit_to_menu(false);
				exit = true;
				
				SDL_Delay(sdl->ENTER_WAIT);
			}
			else if(selection.return_vert() == 2)
			{
				// Open the high score screen.
				display_high_scores(sdl, player, false);
				
				update_screen = true;
				SDL_Delay(sdl->ENTER_WAIT);
			}
            else if(selection.return_vert() == 3)
            {
                // Open the instructions screen.
                display_instructions(sdl);
                
                update_screen = true;
                SDL_Delay(sdl->ENTER_WAIT);
            }
			else if(selection.return_vert() == 4)
			{
				// Quit to OS.
				sdl->set_quitSDL();
				exit = true;
			}
		}
		
		if(update_screen == true)
		{
			screen_data.update_start_screen(sdl, &selection);
			SDL_Flip(sdl->return_screen());			
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}	
}

Start_Screen::Start_Screen()
{
	background = IMG_Load("./Graphics/start_screen/background.png");
	miner_sdl_logo = IMG_Load("./Graphics/start_screen/miner_sdl_logo.png");
	new_game_button = IMG_Load("./Graphics/start_screen/new_game.png");
	load_game_button = IMG_Load("./Graphics/start_screen/load_game.png");
	high_score_button = IMG_Load("./Graphics/start_screen/high_score.png");
	instructions_button = IMG_Load("./Graphics/start_screen/instructions.png");
    copyright_info = IMG_Load("./Graphics/start_screen/copyright.png");
	exit_game_button = IMG_Load("./Graphics/start_screen/exit_game.png");
	
	arrow = IMG_Load("./Graphics/start_screen/arrow.png");
}

Start_Screen::~Start_Screen()
{
	SDL_FreeSurface(background);
	SDL_FreeSurface(miner_sdl_logo);
	SDL_FreeSurface(new_game_button);
	SDL_FreeSurface(load_game_button);
	SDL_FreeSurface(high_score_button);
	SDL_FreeSurface(instructions_button);
    SDL_FreeSurface(copyright_info);
	SDL_FreeSurface(exit_game_button);
	
	SDL_FreeSurface(arrow);
}

void Start_Screen::update_start_screen(SDL_Objects *sdl, Selection_Arrow *selection)
{
	sdl->apply_surface(0, 0, background, sdl->return_screen());
	sdl->apply_surface(32, 32, miner_sdl_logo, sdl->return_screen());

	sdl->apply_surface(480, 136, new_game_button, sdl->return_screen());
	sdl->apply_surface(480, 200, load_game_button, sdl->return_screen());
	sdl->apply_surface(480, 264, high_score_button, sdl->return_screen());
    sdl->apply_surface(480, 328, instructions_button, sdl->return_screen());
	sdl->apply_surface(480, 392, exit_game_button, sdl->return_screen());
    
    sdl->apply_surface(15, 450, copyright_info, sdl->return_screen());
	
	sdl->apply_surface(selection->return_arrow_x(), selection->return_arrow_y(), arrow, sdl->return_screen());
}

void Start_Screen::animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() + 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp - 16;
		sdl->apply_surface(0, 0, background, sdl->return_screen());
        
        sdl->apply_surface(32, 32, miner_sdl_logo, sdl->return_screen());
        
        sdl->apply_surface(480, 136, new_game_button, sdl->return_screen());
        sdl->apply_surface(480, 200, load_game_button, sdl->return_screen());
        sdl->apply_surface(480, 264, high_score_button, sdl->return_screen());
        sdl->apply_surface(480, 328, instructions_button, sdl->return_screen());
        sdl->apply_surface(480, 392, exit_game_button, sdl->return_screen());
        
		sdl->apply_surface(selection->return_arrow_x(), temp, arrow, sdl->return_screen());
		
        SDL_UpdateRect(sdl->return_screen(), 410, 200, 70, 280);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}	
}

void Start_Screen::animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() - 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp + 16;
		sdl->apply_surface(0, 0, background, sdl->return_screen());
        
        sdl->apply_surface(32, 32, miner_sdl_logo, sdl->return_screen());
        
        sdl->apply_surface(480, 136, new_game_button, sdl->return_screen());
        sdl->apply_surface(480, 200, load_game_button, sdl->return_screen());
        sdl->apply_surface(480, 264, high_score_button, sdl->return_screen());
        sdl->apply_surface(480, 328, instructions_button, sdl->return_screen());
        sdl->apply_surface(480, 392, exit_game_button, sdl->return_screen());
        
		sdl->apply_surface(selection->return_arrow_x(), temp, arrow, sdl->return_screen());
		
        SDL_UpdateRect(sdl->return_screen(), 410, 200, 70, 280);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}		
}
