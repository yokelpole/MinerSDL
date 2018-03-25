/*
 mine.cpp
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
 Main file for the mining section of the game.
  
 This file accepts the keyboard input for what the player does within
 the mine alongside various other things.
  
 For all graphical work in the mine, see "sdl_functions"
*/

#include <iostream>

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "sdl_functions.h"
#include "classes.h"
#include "mine.h"
#include "timer.h"
#include "popup_menu.h"
#include "high_scores.h"

void mine_function(PlayerData *player, SDL_Objects *sdl, MineData *mine)
{
	// To catch the user's input.
	SDL_Event user_input;
	
	// Timer to allow for keyboard action tracking
	Timer keyboard_timer;
	
    // Enum to keep track of which direction the player is going.
    direction player_direction = NONE;
    
	// Update the HUD with current status.
	sdl->update_status_text("You descend into the mine...");
	
	// Restore the player to the origin of the mine
	player->change_location(0, 0, mine, sdl);
	
	// Update the graphics for the first refresh.
	sdl->update_mine_graphics(player, mine, player_direction);
	sdl->display_hud(player);
	SDL_Flip(sdl->return_screen());
	
	// Testing stuff...
	bool exit = false;
	bool update_screen = false;
		
	// Wait for the user's input.	
	while(!exit && !sdl->return_quit_to_menu())
	{	
		Uint8 *keystate = SDL_GetKeyState(NULL);
		
        // Start/stop the animation loop.
        update_screen = false;         
        
        //While there's events to handle
        while( SDL_PollEvent( &user_input ) )
        {
			// Quit if the user chooses to close the window.
			if(user_input.type == SDL_QUIT)
			{
				sdl->set_quitSDL();
				exit = true;
			}
			else if(user_input.type == SDL_KEYUP)
			{
				// Check to see if dynamite is counting down.
				if(player->dynamite_countdown(mine))
				{
					// Above returns true if player was too close to the blast.
					sdl->update_status_text("You were too close to the blast!");
					player->change_health(-50);
				}					
			}
        }	
        
		// Return to the town if the elevator moves to the top.
		if((player->get_location_x() == 0) && (player->get_location_y() == -1))
		{
			exit = true;
		}
		
		if(keystate[ SDLK_DOWN ])
		{
			player->change_location((player->get_location_x()), (player->get_location_y() + 1), mine, sdl);
			update_screen = true;
            player_direction = DOWN;
		}
		else if(keystate[ SDLK_UP ])
		{
			player->change_location((player->get_location_x()), (player->get_location_y() - 1), mine, sdl);
			update_screen = true;
            player_direction = UP;
		}
		else if(keystate[ SDLK_LEFT ])
		{			
			player->change_location((player->get_location_x() - 1), player->get_location_y(), mine, sdl);
			update_screen = true;
            player_direction = LEFT;
		}
		else if(keystate[ SDLK_RIGHT ])
		{			
			player->change_location((player->get_location_x() + 1), player->get_location_y(), mine, sdl);
			update_screen = true;
            player_direction = RIGHT;
		}	
		// Activate the dynamite, if the player has any.					
		else if(keystate[ SDLK_d ])
		{
			if(player->get_has_dynamite()
				&& mine->get_contents(player->get_location_x(), player->get_location_y()) != ELEVATOR)
			{
				player->dynamite_prime(player->get_location_x(), player->get_location_y(), mine);
				sdl->update_status_text("You light the dynamite. RUN!");
				SDL_Delay(125);
			}
			else if(!player->get_has_dynamite())
			{
				sdl->update_status_text("You don't have any dynamite!");
			}
			
			update_screen = true;
		}
		// Allow the player to view the map.
		else if(keystate[ SDLK_m ])
		{
			SDL_Delay(sdl->KEYPRESS_WAIT);
			mine_show_map(mine, sdl, player);
			
			update_screen = true;
		}
		// Allow the player to instantly travel to the lowest level accessible
		// to the elevator
		else if(keystate[ SDLK_b ])
		{
			SDL_Delay(sdl->KEYPRESS_WAIT);
						
			if(move_elevator_to_bottom(sdl, mine, player))
			{
				sdl->update_status_text("To the depths!");
			}
			else
			{
				sdl->update_status_text("You can't do that now!");
			}
			
			update_screen = true;
		}
		else if(keystate [ SDLK_t ])
		{
			SDL_Delay(sdl->KEYPRESS_WAIT);
			
			if(move_elevator_to_top(sdl, mine, player))
			{
				sdl->update_status_text("Daylight!");
			}
			else
			{
				sdl->update_status_text("You can't do that now!");
			}
			
			update_screen = true;
		}				
		else if(keystate[ SDLK_ESCAPE ] || keystate[SDLK_BACKSPACE])
		{	
			SDL_Delay(sdl->ENTER_WAIT);
			display_confirm_quit(sdl);
			update_screen = true;
		}
		
        // Countdown for the recently found minerals
		if(mine->return_recently_found_countdown() >= 0)
		{
			update_screen = true;
			mine->count_recently_found();
		}
		
		// Apply the graphics on screen and update them.
		if(update_screen)
		{
			sdl->update_mine_graphics(player, mine, player_direction);
			sdl->display_hud(player);
			SDL_Flip(sdl->return_screen());
            SDL_Delay(sdl->MINE_ANIMATION_WAIT);
            
            // Animate 'between' still frames.
            if(player_direction != NONE)
            {
                if(mine->return_recently_found_countdown() >= 0)
                {
                    update_screen = true;
                    mine->count_recently_found();
                }                
                
                sdl->update_mine_graphics(player, mine, NONE);
                player_direction = NONE;
                
                sdl->display_hud(player);
                SDL_Flip(sdl->return_screen());
                SDL_Delay(sdl->MINE_ANIMATION_WAIT);
            }
		}
		
		// Check to see if the player's health has fallen below 0.
		if(!player->check_health(sdl))
		{
			// Show the death screen, then go to the main menu.
			display_dead_message(sdl);
			display_high_scores(sdl, player, true);			
			sdl->set_quit_to_menu(true);
			return;
		}	
		
		// Check to see if the player's money has fallen below 0.
		if(player->get_money() < 0)
		{
			// Show the broke screen, then go to the main menu.
			display_broke_message(sdl);
			sdl->set_quit_to_menu(true);
			return;
		}	        
        	
		SDL_Delay(sdl->SDL_WAIT);	// Delay the loop so the CPU isn't maxed out.
	}
}

// Show the player the map of where the diamond is.
void mine_show_map(MineData *mine, SDL_Objects *sdl, PlayerData *player)
{
	SDL_Surface *minimap;
	SDL_Surface *minimap_explored_area;
	SDL_Surface *player_location;
	
	minimap = IMG_Load("./Graphics/mine/map/cheat_map.png");
	minimap_explored_area = IMG_Load("./Graphics/mine/map/explored_pixel.png");
	player_location = IMG_Load("./Graphics/mine/map/player_indicator.png");
	
	// Display the map on screen.
	sdl->apply_surface(0, 0, minimap, sdl->return_screen());
	
	// Show where the player has explored.
	int x_position = 192;
	int y_position = 0;
	
	for(int y = 0; y <= mine->get_map_y(); y++)
	{
		for(int x = 0; x <= mine->get_map_x(); x++)
		{
			if(mine->get_explored(x, y))
			{
				sdl->apply_surface(x_position, y_position, minimap_explored_area, sdl->return_screen());
			}
			
			x_position += 2;
		}
		x_position = 192;
		y_position += 2;
	}
	
	// Show where the player is currently located.
	x_position = 192 + (player->get_location_x() * 2) - 2;
	y_position = (player->get_location_y() * 2) - 2;
	sdl->apply_surface(x_position, y_position, player_location, sdl->return_screen());
	
	// Update the screen and wait for user input.
	SDL_Flip(sdl->return_screen());
	wait_for_keypress(sdl);
	
	SDL_FreeSurface(minimap);
	SDL_FreeSurface(minimap_explored_area);
}



// Wait for a user keypress to exit the map screen.
void wait_for_keypress(SDL_Objects *sdl)
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

// Move the elevator to the lowest level explored if the player is within
// the elevator.
bool move_elevator_to_bottom(SDL_Objects *sdl, MineData *mine, PlayerData *player)
{
	// Check to see if the player is in the elevator.
	if(player->get_location_x() == 0)
	{
		// Variable to keep track of the lowest area the player has explored
		// adjacent to the elevator.
		int lowest_explored = 0;
		
		for(int y = 0; y <= mine->get_map_y(); y++)
		{
			if(mine->get_explored(1, y))
			{
				lowest_explored = y;
			}
		}
		
		if(lowest_explored <= player->get_money()
			&& player->get_location_y() < lowest_explored)
		{
			if(player->get_location_y() < lowest_explored)
			{
				// Charge the player for the elevator usage.
				player->change_money(-1 * (lowest_explored - player->get_location_y()));
			}
			
			player->change_location(0, lowest_explored, mine, sdl);
			
			// Say that the player can indeed move down.
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

// Move the elevator to the very top level provided the player is within
// the elevator.
bool move_elevator_to_top(SDL_Objects *sdl, MineData *mine, PlayerData *player)
{
	// Check to see if the player is in the elevator
	if(player->get_location_x() == 0 && player->get_location_y() != 0)
	{
		player->change_location(0, 0, mine, sdl);
		
		return true;
	}
	else
	{
		return false;
	}
}
