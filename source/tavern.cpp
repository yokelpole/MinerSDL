/*
 tavern.cpp
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
 Functions dealing with the tavern section of the game.
  
 Here the player can see Mimi or get a tip as to where the diamond is.
  
 The game can be finished from this area, as if the player meets
 conditions desirable to Mimi ($2500+ & diamond in possession) the
 game will end.
*/

#include <ctime>		// Allows generation of psuedorandom numbers.
#include <cstdlib>		// Allows for rand()

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "sdl_functions.h"
#include "classes.h"
#include "tavern.h"
#include "timer.h"
#include "endgame_screens.h"
#include "high_scores.h"

void tavern(PlayerData *player, MineData *mine, SDL_Objects *sdl)
{
	// Initialize the tavern class object.
	Tavern_Objects tavern_data;
	
	// Initialize the selection arrow
	Selection_Arrow tavern_arrow(1, 5, 0, 64);		// Initializes the arrow object with six places to go up/down.
	tavern_arrow.set_arrow_initial(512, 0);
	
	// Initialize an event to track the user's input
	SDL_Event user_input;
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
	
	// Welcome the player to the store screen.
	sdl->update_status_text("It's happy hour!");
	
	// Update the screen for the first time.
	tavern_data.update_tavern_graphics(sdl, player, &tavern_arrow);
	sdl->display_hud(player);
	SDL_Flip(sdl->return_screen());
	
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
			if(tavern_arrow.move_down())
			{
				tavern_data.animate_arrow_down(sdl, &tavern_arrow);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_UP])
		{
			if(tavern_arrow.move_up())
			{
				tavern_data.animate_arrow_up(sdl, &tavern_arrow);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER])
		{
			if(tavern_arrow.return_vert() == 0)
			{
				// Let the player try to see Mimi
				tavern_data.update_tavern_graphics_no_overlay(sdl, player, &tavern_arrow);
				if(tavern_data.see_mimi(player, sdl, mine))
				{
					exit = true;
					sdl->set_quit_to_menu(true);
				}
				else
				{				
					tavern_data.update_tavern_graphics(sdl, player, &tavern_arrow);
				}
			}
			else if(tavern_arrow.return_vert() == 1)
			{
				// Let the player try to get a cheap hint.
				if(player->get_money() >= 250)
				{	
					player->change_money(-250);
					tavern_data.get_tip(player, mine, sdl, CHEAP);
				}
				else
				{
					sdl->update_status_text("You can't afford that tip!");
				}
				
				update_screen = true;
			}
			else if(tavern_arrow.return_vert() == 2)
			{
				// Let the player try to get a medium hint.
				if(player->get_money() >= 750)
				{
					player->change_money(-750);
					tavern_data.get_tip(player, mine, sdl, GOOD);
				}
				else
				{
					sdl->update_status_text("You can't afford that tip!");
				}
				
				update_screen = true;
			}
			else if(tavern_arrow.return_vert() == 3)
			{
				// Let the player try to get the best tip
				if(player->get_money() >= 1500)
				{	
					player->change_money(-1500);
					tavern_data.get_tip(player, mine, sdl, BEST);
				}
				else
				{
					sdl->update_status_text("You can't afford that tip!");
				}
				
				update_screen = true;
			}
			else if(tavern_arrow.return_vert() == 4)
			{
				// Leave the tavern.
				sdl->update_status_text("You've had enough! Get out!");
				sdl->display_hud(player);
				SDL_Delay(sdl->ENTER_WAIT);
				exit = true;
			}
			
			if(!exit)
			{
				update_screen = true;
			}
		}
		else if(keystate[SDLK_ESCAPE] || keystate[SDLK_BACKSPACE])
		{
			SDL_Delay(sdl->ENTER_WAIT);	
			sdl->update_status_text("You've had enough! Get out!");
			sdl->display_hud(player);
			exit = true;
		}
			
		// Apply the graphics on screen and update them.
		if(update_screen)
		{
			tavern_data.update_tavern_graphics(sdl, player, &tavern_arrow);	
			SDL_UpdateRect(sdl->return_screen(), 500, 0, 70, 380);
			sdl->display_hud(player);
			SDL_Flip(sdl->return_screen());
		}
		
		SDL_Delay(sdl->SDL_WAIT);			
	}
}

Tavern_Objects::Tavern_Objects()
{
	// Background graphic for the tavern.
	tavern_graphic = IMG_Load("./Graphics/tavern/tavern_screen.png");
	
	// Individual buttons for the tavern options.
	see_mimi_button = IMG_Load("./Graphics/tavern/see_mimi_button.png");
	tavern_transparency = IMG_Load("./Graphics/tavern/tavern_transparency.png");
	cheap_tip_button = IMG_Load("./Graphics/tavern/cheap_tip_button.png");
	good_tip_button = IMG_Load("./Graphics/tavern/good_tip_button.png");
	best_tip_button = IMG_Load("./Graphics/tavern/best_tip_button.png");
	exit_tavern_button = IMG_Load("./Graphics/tavern/exit_tavern.png");
	
	// Pointing arrow for the menu.
	arrow_graphic = IMG_Load("./Graphics/tavern/arrow.png");
	
	// Load the minimap used in the tips.
	minimap = IMG_Load("./Graphics/tavern/cheat_map.png");
	minimap_big_overlay = IMG_Load("./Graphics/tavern/large_overlay.png");
	minimap_medium_overlay = IMG_Load("./Graphics/tavern/medium_overlay.png");
	minimap_small_overlay = IMG_Load("./Graphics/tavern/small_overlay.png");
	minimap_explored_area = IMG_Load("./Graphics/tavern/explored_pixel.png");
	
	// Mimi graphics.
	mimi_happy = IMG_Load("./Graphics/tavern/mimi_happy.png");
	mimi_sad = IMG_Load("./Graphics/tavern/mimi_unimpressed.png");
	mimi_talk_window = IMG_Load("./Graphics/tavern/mimi_talk_window.png");
	
	// Load the fonts.
	header_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
	display_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 20);
}

Tavern_Objects::~Tavern_Objects()
{
	// Free the images that have been loaded.
	SDL_FreeSurface(tavern_graphic);
	
	SDL_FreeSurface(see_mimi_button);
	SDL_FreeSurface(tavern_transparency);
	SDL_FreeSurface(cheap_tip_button);
	SDL_FreeSurface(good_tip_button);
	SDL_FreeSurface(best_tip_button);
	SDL_FreeSurface(exit_tavern_button);
	
	SDL_FreeSurface(arrow_graphic);
	
	SDL_FreeSurface(minimap);
	SDL_FreeSurface(minimap_big_overlay);
	SDL_FreeSurface(minimap_medium_overlay);
	SDL_FreeSurface(minimap_small_overlay);
	SDL_FreeSurface(minimap_explored_area);
	
	SDL_FreeSurface(mimi_happy);
	SDL_FreeSurface(mimi_sad);
	SDL_FreeSurface(mimi_talk_window);
	
	// Free the fonts that have been loaded.
	TTF_CloseFont(header_font);
	TTF_CloseFont(display_font);
}

// Update the objects on the screen.
void Tavern_Objects::update_tavern_graphics(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *tavern_arrow)
{
	// Display the background image and overlay.
	sdl->apply_surface(0, 0, tavern_graphic, sdl->return_screen());
	sdl->apply_surface(5, 5, tavern_transparency, sdl->return_screen());
	
	// Display the Tavern's name
	sdl->apply_text(90, 10, "THE RUSTY PICKAXE", header_font, sdl->return_screen());    
    
	// Display the buttons on the sidebar.
	sdl->apply_surface(576, 0, see_mimi_button, sdl->return_screen());
	sdl->apply_surface(576, 64, cheap_tip_button, sdl->return_screen());
	sdl->apply_surface(576, 128, good_tip_button, sdl->return_screen());
	sdl->apply_surface(576, 192, best_tip_button, sdl->return_screen());
	sdl->apply_surface(576, 256, exit_tavern_button, sdl->return_screen());

	// Update the location of the selection arrow.
	sdl->apply_surface(tavern_arrow->return_arrow_x(), tavern_arrow->return_arrow_y(),
						arrow_graphic, sdl->return_screen());
						
	if(tavern_arrow->return_vert() == 0)
	{
		// Let the player try to see Mimi.
		sdl->apply_text(10, 75, "Try to see Mimi.", display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "Mimi will only fall for a man with a lot", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "of money and a diamond.", display_font, sdl->return_screen());
		sdl->apply_text(10, 200, "Care to try your luck?", display_font, sdl->return_screen());
	}
	else if(tavern_arrow->return_vert() == 1)
	{
		// Let the player know how much a cheap tip will cost.
		sdl->apply_text(10, 75, "Get a cheap tip about the diamond!", display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "For a low, low cost you can see (very)", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "roughly where the diamond that will", display_font, sdl->return_screen());
		sdl->apply_text(10, 175, "capture Mimi's heart is!", display_font, sdl->return_screen());
		sdl->apply_text(150, 300, "COST: $250", header_font, sdl->return_screen());
	}
	else if(tavern_arrow->return_vert() == 2)
	{
		// Let the player know how much a medium tip will cost.
		sdl->apply_text(10, 75, "Get a better tip about the diamond!", display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "For a slightly higher price, you can see", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "where Mimi's favourite diamond is within", display_font, sdl->return_screen());
		sdl->apply_text(10, 175, "the mine. You don't want to look cheap,", display_font, sdl->return_screen());
		sdl->apply_text(10, 200, "do you?", display_font, sdl->return_screen());
		sdl->apply_text(150, 300, "COST: $750", header_font, sdl->return_screen());		
	}
	else if(tavern_arrow->return_vert() == 3)
	{
		// Let the player know how much an expensive tip will cost.
		sdl->apply_text(10, 75, "Get the best tip about the diamond!", display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "Using the latest and greatest in mining", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "technology, we can show you with pinpoint", display_font, sdl->return_screen());
		sdl->apply_text(10, 175, "precision where the diamond is.", display_font, sdl->return_screen());
		sdl->apply_text(10, 200, "As this is a premier service, the cost is", display_font, sdl->return_screen());
		sdl->apply_text(10, 225, "accordingly set.", display_font, sdl->return_screen());
		sdl->apply_text(150, 300, "COST: $1500", header_font, sdl->return_screen());
	}
	else if(tavern_arrow->return_vert() == 4)
	{
		// Let the player leave the tavern.
		sdl->apply_text(10, 75, "Leave the pub.", display_font, sdl->return_screen());
		sdl->apply_text(10, 125, "Mimi won't talk to you, eh?", display_font, sdl->return_screen());
		sdl->apply_text(10, 150, "Better luck next time!", display_font, sdl->return_screen());
	}
}

// Update the objects on the screen.
void Tavern_Objects::update_tavern_graphics_no_overlay(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *tavern_arrow)
{
	// Display the background image and overlay.
	sdl->apply_surface(0, 0, tavern_graphic, sdl->return_screen());
	
	// Display the buttons on the sidebar.
	sdl->apply_surface(576, 0, see_mimi_button, sdl->return_screen());
	sdl->apply_surface(576, 64, cheap_tip_button, sdl->return_screen());
	sdl->apply_surface(576, 128, good_tip_button, sdl->return_screen());
	sdl->apply_surface(576, 192, best_tip_button, sdl->return_screen());
	sdl->apply_surface(576, 256, exit_tavern_button, sdl->return_screen());

	// Update the location of the selection arrow.
	sdl->apply_surface(tavern_arrow->return_arrow_x(), tavern_arrow->return_arrow_y(),
						arrow_graphic, sdl->return_screen());
}

// Animate the movement of the arrow.
void Tavern_Objects::animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() + 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp - 16;
        
        // Update the background
		sdl->apply_surface(0, 0, tavern_graphic, sdl->return_screen());
        sdl->apply_surface(5, 5, tavern_transparency, sdl->return_screen());
        
        // Display the Tavern's name
        sdl->apply_text(90, 10, "THE RUSTY PICKAXE", header_font, sdl->return_screen());    
        
        // Display the buttons on the sidebar.
        sdl->apply_surface(576, 0, see_mimi_button, sdl->return_screen());
        sdl->apply_surface(576, 64, cheap_tip_button, sdl->return_screen());
        sdl->apply_surface(576, 128, good_tip_button, sdl->return_screen());
        sdl->apply_surface(576, 192, best_tip_button, sdl->return_screen());
        sdl->apply_surface(576, 256, exit_tavern_button, sdl->return_screen());

        sdl->apply_surface(selection->return_arrow_x(), temp, arrow_graphic, sdl->return_screen());
		SDL_UpdateRect(sdl->return_screen(), 515, 0, 55, 380);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}
void Tavern_Objects::animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() - 64;

	while(temp != selection->return_arrow_y())
	{
		temp = temp + 16;

        // Update the background
		sdl->apply_surface(0, 0, tavern_graphic, sdl->return_screen());
        sdl->apply_surface(5, 5, tavern_transparency, sdl->return_screen());
        
        // Display the Tavern's name
        sdl->apply_text(90, 10, "THE RUSTY PICKAXE", header_font, sdl->return_screen());    
        
        // Display the buttons on the sidebar.
        sdl->apply_surface(576, 0, see_mimi_button, sdl->return_screen());
        sdl->apply_surface(576, 64, cheap_tip_button, sdl->return_screen());
        sdl->apply_surface(576, 128, good_tip_button, sdl->return_screen());
        sdl->apply_surface(576, 192, best_tip_button, sdl->return_screen());
        sdl->apply_surface(576, 256, exit_tavern_button, sdl->return_screen());

		sdl->apply_surface(selection->return_arrow_x(), temp, arrow_graphic, sdl->return_screen());

		SDL_UpdateRect(sdl->return_screen(), 515, 0, 55, 380);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}
	
// Functions that respond to player actions.
// Return true if the player wins the game.
bool Tavern_Objects::see_mimi(PlayerData *player, SDL_Objects *sdl, MineData *mine)
{	
	srand( time(NULL) );		// Seed the random number generator.
								// Used for Mimi's responses to the player.
	
	bool exit = false;
								
	int response_number = 0;
	
	response_number = rand() % 5;

	if(player->get_money() < 2500)
	{
		// Pop up screen of mimi telling the player to get lost.
		sdl->apply_surface(0,0, mimi_sad, sdl->return_screen());
		sdl->apply_surface(36, 192, mimi_talk_window, sdl->return_screen());
			
		if(response_number == 0)
		{
			sdl->apply_text(48, 212, "Yeah, as if!", display_font, sdl->return_screen());	
		}
		else if(response_number == 1)
		{
			sdl->apply_text(48, 212, "I don't know why I'm even talking to you.", display_font, sdl->return_screen());	
		}
		else if(response_number == 2)
		{
			sdl->apply_text(48, 212, "Romance is dead. I only want money.", display_font, sdl->return_screen());	
		}

		else if(response_number == 3)
		{
			sdl->apply_text(48, 212, "Get away from me. Now.", display_font, sdl->return_screen());	
		}
		else if(response_number == 4)
		{
			sdl->apply_text(48, 212, "Buy me a drink. Then leave me alone.", display_font, sdl->return_screen());	
		}
	}
	else if(player->get_money() >= 2500 && player->get_money() < 5000
			&& !player->get_has_diamond())
	{
		// Pop up screen of mimi seeming mildly interested.
		sdl->apply_surface(0,0, mimi_happy, sdl->return_screen());
		sdl->apply_surface(36, 192, mimi_talk_window, sdl->return_screen());
		
		if(response_number == 0)
		{
			sdl->apply_text(48, 212, "Hmm... You look OK. Not great, but OK.", display_font, sdl->return_screen());	
		}
		else if(response_number == 1)
		{
			sdl->apply_text(48, 212, "I guess I can tell people I know you.", display_font, sdl->return_screen());	
		}
		else if(response_number == 2)
		{
			sdl->apply_text(48, 212, "Work harder, and I could be yours!", display_font, sdl->return_screen());	
		}
		else if(response_number == 3)
		{
			sdl->apply_text(48, 212, "I guess you're not THAT disgusting.", display_font, sdl->return_screen());	
		}
		else if(response_number == 4)
		{
			sdl->apply_text(48, 212, "Buy me a drink.", display_font, sdl->return_screen());	
		}		
	}
	else if(player->get_money() > 5000 && player->get_has_diamond() == false)
	{
		// Mimi's graphic.
		sdl->apply_surface(0,0, mimi_happy, sdl->return_screen());
		sdl->apply_surface(36, 192, mimi_talk_window, sdl->return_screen());		
		
		// Pop up screen of mimi asking the player for the diamond.
		if(response_number == 0)
		{
			sdl->apply_text(48, 212, "They say diamonds are a girl's best friend.", display_font, sdl->return_screen());	
		}
		else if(response_number == 1)
		{
			sdl->apply_text(48, 212, "Put a ring on my hand and I'm yours!", display_font, sdl->return_screen());	
		}
		else if(response_number == 2)
		{
			sdl->apply_text(48, 212, "I'd be yours, but I need a ring first!", display_font, sdl->return_screen());	
		}
		else if(response_number == 3)
		{
			sdl->apply_text(48, 212, "No diamond yet? That's too bad.", display_font, sdl->return_screen());	
		}
		else if(response_number == 4)
		{
			sdl->apply_text(48, 212, "Except for one thing, you've got it all.", display_font, sdl->return_screen());	
		}				
	}
	else if(player->get_money() >= 2500 && player->get_money() < 5000 && player->get_has_diamond() == true)
	{
		// Let the player finish the game, but get the bad ending.
		sdl->apply_surface(0,0, mimi_happy, sdl->return_screen());
		sdl->apply_surface(36, 192, mimi_talk_window, sdl->return_screen());

		sdl->apply_text(48, 212, "You're not rich, but this IS a nice ring.", display_font, sdl->return_screen());	
		SDL_Flip(sdl->return_screen());	
		wait_for_enter(sdl);

		// Show the ending screen, followed by the high score then quit to menu.
		display_ending(sdl, player, mine);
		display_high_scores(sdl, player, true);
		
		exit = true;
	}
	else if(player->get_money() > 5000 && player->get_has_diamond() == true)
	{
		// Let the player have the best ending.
		sdl->apply_surface(0,0, mimi_happy, sdl->return_screen());
		sdl->apply_surface(36, 192, mimi_talk_window, sdl->return_screen());		
		
		sdl->apply_text(48, 212, "I'm all yours!", display_font, sdl->return_screen());	
		SDL_Flip(sdl->return_screen());	
		wait_for_enter(sdl);
		
		// Show the ending screen, followed by the high score then quit to menu.
		display_ending(sdl, player, mine);
		display_high_scores(sdl, player, true);
		
		exit = true;
	}
	
	if(exit != true)
	{
		SDL_Flip(sdl->return_screen());
	
		wait_for_enter(sdl);
	}

	return(exit);
}

void Tavern_Objects::get_tip(PlayerData *player, MineData *mine, SDL_Objects *sdl, tip_amount tip)
{
	show_map(mine, sdl);
	display_tip(mine, sdl, tip);
		
	sdl->display_hud(player);	
	SDL_Flip(sdl->return_screen());
	
	// Wait for the player to press enter before leaving the screen.
	wait_for_enter(sdl);
}

// Show the player the map of where the diamond is.
void Tavern_Objects::show_map(MineData *mine, SDL_Objects *sdl)
{
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
}

// Display the tip on the map. 
void Tavern_Objects::display_tip(MineData *mine, SDL_Objects *sdl, tip_amount tip)
{
	if(tip == CHEAP)
	{
		if(mine->get_diamond_x() <= 96 && mine->get_diamond_y() <= 96)
		{
			sdl->apply_surface(192, 0, minimap_big_overlay, sdl->return_screen());
		}
		else if(mine->get_diamond_x() <= 96 && mine->get_diamond_y() > 96)
		{
			sdl->apply_surface(192, 192, minimap_big_overlay, sdl->return_screen());
		}
		else if(mine->get_diamond_x() > 96 && mine->get_diamond_y() <= 96)
		{
			sdl->apply_surface(384, 0, minimap_big_overlay, sdl->return_screen());
		}
		else if(mine->get_diamond_x() > 96 && mine->get_diamond_y() > 96)
		{
			sdl->apply_surface(384, 192, minimap_big_overlay, sdl->return_screen());
		}	
	}
	else if(tip == GOOD)
	{
		int random_area_x = 0;
		int random_area_y = 0;
		
		srand( time(NULL) );		// Seed the random number generator.
		
		random_area_x = rand() % 24;
		random_area_y = rand() % 48;
		
		sdl->apply_surface(192 + ((mine->get_diamond_x() * 2) - random_area_x), ((mine->get_diamond_y() * 2) - random_area_y), minimap_medium_overlay, sdl->return_screen());
	}
	else if(tip == BEST)
	{
		sdl->apply_surface(192 + ((mine->get_diamond_x() * 2) - 3), ((mine->get_diamond_y() * 2) - 3), minimap_small_overlay, sdl->return_screen());
	}
}

void Tavern_Objects::wait_for_enter(SDL_Objects *sdl)
{
	// Function waits for player to press enter key before progressing.
	bool stop_loop = false;
	
	SDL_Event user_input;
	Timer keyboard_timer;
	
	// Stop a keypress from earlier being carried over.
	SDL_Delay(sdl->ENTER_WAIT);
	
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
