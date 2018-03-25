/*
 store_functions.cpp
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
 Functions that deal with the store.

 Here the player is able to purchase various items that will be of 
 aid within the mine.
*/

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "sdl_functions.h"
#include "classes.h"
#include "store_functions.h"
#include "timer.h"

void store(PlayerData *player, SDL_Objects *sdl)
{
	// Create the store object to store data in.
	Store_Objects store_data;
	
	// To catch the user's input.
	SDL_Event user_input;
	
	// Create the selection box.
	Selection_Arrow store_selection(3, 3, 160, 128);
	store_selection.set_arrow_initial(162, 8);
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
	
	// Welcome the player to the store screen.
	sdl->update_status_text("Welcome to Sleazy Pete's!");
	
	// Initially show the screen.
	store_data.update_store_graphics(sdl, player, &store_selection);
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
			if(store_selection.move_down() && store_selection.return_vert() < 2)
			{
				store_data.animate_box_down(sdl, &store_selection, player);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_UP])
		{
			if(store_selection.move_up() && store_selection.return_vert() != 1)
			{
				store_data.animate_box_up(sdl, &store_selection, player);
			}
			else
			{					
				SDL_Delay(100);
			}
			
			update_screen = true;
		}
		else if(keystate[SDLK_LEFT])
		{
			if(store_selection.move_left() && store_selection.return_vert() != 2)
			{
				store_data.animate_box_left(sdl, &store_selection, player);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_RIGHT])
		{
			if(store_selection.move_right() && store_selection.return_vert() != 2)
			{
				store_data.animate_box_right(sdl, &store_selection, player);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER])
		{
			if(store_selection.return_horiz() == 0 && store_selection.return_vert() == 0)
			{
				// Purchase the shovel.
				if(!player->get_has_shovel() && player->get_money() >= 250)
				{
					player->change_has_shovel(true);
					player->change_money(-250);
				}
				else if(player->get_has_shovel())
				{
					sdl->update_status_text("You already own that!");
				}
				else
				{
					sdl->update_status_text("You can't afford that!");
				}
			}
			else if(store_selection.return_horiz() == 1 && store_selection.return_vert() == 0)
			{
				// Purchase the pickaxe.
				if(!player->get_has_axe() && player->get_money() >= 200)
				{
					player->change_has_axe(true);
					player->change_money(-200);
				}
				else if(player->get_has_axe())
				{
					sdl->update_status_text("You already own that!");
				}
				else
				{
					sdl->update_status_text("You can't afford that!");
				}
			}	
			else if(store_selection.return_horiz() == 2 && store_selection.return_vert() == 0)
			{
				// Purchase the bucket.
				if(!player->get_has_bucket() && player->get_money() >= 250)
				{
					player->change_has_bucket(true);
					player->change_money(-250);
				}
				else if(player->get_has_bucket())
				{
					sdl->update_status_text("You already own that!");
				}
				else
				{
					sdl->update_status_text("You can't afford that!");
				}
			}
			else if(store_selection.return_horiz() == 0 && store_selection.return_vert() == 1)
			{
				// Purchase dynamite.
				if(!player->get_has_dynamite() && player->get_money() >= 500)
				{
					player->change_has_dynamite(true);
					player->change_money(-500);
				}
				else if(player->get_has_dynamite())
				{
					sdl->update_status_text("You already own that!");
				}
				else
				{
					sdl->update_status_text("You can't afford that!");
				}
			}
			else if(store_selection.return_horiz() == 1 && store_selection.return_vert() == 1)
			{
				// Purchase the flashlight.
				if(!player->get_has_flashlight() && player->get_money() >= 300)
				{
					player->change_has_flashlight(true);
					player->change_money(-300);
				}
				else if(player->get_has_flashlight())
				{
					sdl->update_status_text("You already own that!");
				}
				else
				{
					sdl->update_status_text("You can't afford that!");
				}
			}
			else if(store_selection.return_horiz() == 2 && store_selection.return_vert() == 1)
			{
				// Purchase the hard hat.
				if(!player->get_has_hardhat() && player->get_money() >= 200)
				{
					player->change_has_hardhat(true);
					player->change_money(-200);
				}
				else if(player->get_has_hardhat())
				{
					sdl->update_status_text("You already own that!");
				}				
				else
				{
					sdl->update_status_text("You can't afford that!");
				}
			}
			else if(store_selection.return_vert() == 2)
			{
				sdl->update_status_text("Warranty? What's that?");
				exit = true;
				SDL_Delay(sdl->ENTER_WAIT);
			}
			SDL_Delay(sdl->KEYPRESS_WAIT);
			update_screen = true;
		}
		
		// Allow the player to esc out of the shop.
		if(keystate[SDLK_ESCAPE] || keystate[SDLK_BACKSPACE])
		{
			SDL_Delay(sdl->ENTER_WAIT);				
			sdl->update_status_text("Warranty expires as of right now!");
			update_screen = true;
			exit = true;
		}
			
		// Apply the graphics on screen and update them.
		if(update_screen)
		{
			store_data.update_store_graphics(sdl, player, &store_selection);	
			sdl->display_hud(player);
			SDL_Flip(sdl->return_screen());
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}
}

Store_Objects::Store_Objects()
{
	// Background graphic for the store.
	store_graphic = IMG_Load("./Graphics/store/store.png");
	
	// Graphic for the selection box.
	selection_box = IMG_Load("./Graphics/store/selection_box.png");
	
	// Individiual items' graphics.
	shovel_graphic = IMG_Load("./Graphics/store/shovel.png");
	pickaxe_graphic = IMG_Load("./Graphics/store/pickaxe.png");
	bucket_graphic = IMG_Load("./Graphics/store/bucket.png");
	dynamite_graphic = IMG_Load("./Graphics/store/dynamite.png");
	flashlight_graphic = IMG_Load("./Graphics/store/flashlight.png");
	hardhat_graphic = IMG_Load("./Graphics/store/hardhat.png");
	
	// Fonts used in the shop.
	header_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
	display_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 20);
}

Store_Objects::~Store_Objects()
{
	SDL_FreeSurface(store_graphic);
	SDL_FreeSurface(selection_box);
	SDL_FreeSurface(shovel_graphic);
	SDL_FreeSurface(pickaxe_graphic);
	SDL_FreeSurface(bucket_graphic);
	SDL_FreeSurface(dynamite_graphic);
	SDL_FreeSurface(flashlight_graphic);
	SDL_FreeSurface(hardhat_graphic);
	
	TTF_CloseFont(header_font);
	TTF_CloseFont(display_font);
}

void Store_Objects::update_store_graphics(SDL_Objects *sdl, PlayerData *player, Selection_Arrow *store_selection)
{
	// Update the background graphic.
	sdl->apply_surface(0, 0, store_graphic, sdl->return_screen());
	
	// Update the graphics for the items in the store.
	sdl->apply_surface(178, 24, shovel_graphic, sdl->return_screen());
	sdl->apply_surface(338, 24, pickaxe_graphic, sdl->return_screen());
	sdl->apply_surface(498, 24, bucket_graphic, sdl->return_screen());
	sdl->apply_surface(178, 152, dynamite_graphic, sdl->return_screen());
	sdl->apply_surface(338, 152, flashlight_graphic, sdl->return_screen());
	sdl->apply_surface(498, 152, hardhat_graphic, sdl->return_screen());
	
	// Update the location of the selection box.
	if(store_selection->return_vert() != 2)
	{
		sdl->apply_surface(store_selection->return_arrow_x(), store_selection->return_arrow_y(),
							selection_box, sdl->return_screen());
		// Update the EXIT text
		sdl->apply_colored_text(350, 270, 157, 157, 157, "EXIT", header_font, sdl->return_screen());
	}
	else
	{
		// Update the EXIT text to show that it is selected.
		sdl->apply_colored_text(350, 270, 228, 245, 13, "EXIT", header_font, sdl->return_screen());
		SDL_Delay(sdl->KEYPRESS_WAIT);
	}
							
	// Update the informational text regarding the selected tool.
	if(store_selection->return_horiz() == 0 && store_selection->return_vert() == 0)
	{
		sdl->apply_text(178, 320, "SHOVEL", header_font, sdl->return_screen());
		sdl->apply_text(400, 320, "COST:", header_font, sdl->return_screen());
		sdl->apply_text(500, 320, "$250", header_font, sdl->return_screen());
		sdl->apply_text(178, 350, "Shovels make excavating cheaper.", display_font, sdl->return_screen());
	}
	else if(store_selection->return_horiz() == 1 && store_selection->return_vert() == 0)
	{
		sdl->apply_text(178, 320, "PICKAXE", header_font, sdl->return_screen());
		sdl->apply_text(400, 320, "COST:", header_font, sdl->return_screen());
		sdl->apply_text(500, 320, "$200", header_font, sdl->return_screen());
		sdl->apply_text(110, 350, "Pickaxes allow you to bust through hard granite.", display_font, sdl->return_screen());
	}
	else if(store_selection->return_horiz() == 2 && store_selection->return_vert() == 0)
	{
		sdl->apply_text(178, 320, "BUCKET", header_font, sdl->return_screen());
		sdl->apply_text(400, 320, "COST:", header_font, sdl->return_screen());
		sdl->apply_text(500, 320, "$250", header_font, sdl->return_screen());
		sdl->apply_text(115, 350, "A bucket is the only way to clear spring runoff!", display_font, sdl->return_screen());
	}
	else if(store_selection->return_horiz() == 0 && store_selection->return_vert() == 1)
	{
		sdl->apply_text(178, 320, "DYNAMITE", header_font, sdl->return_screen());
		sdl->apply_text(400, 320, "COST:", header_font, sdl->return_screen());
		sdl->apply_text(500, 320, "$500", header_font, sdl->return_screen());
		sdl->apply_text(150, 350, "Dynamite can clear large areas quickly.", display_font, sdl->return_screen());
	}
	else if(store_selection->return_horiz() == 1 && store_selection->return_vert() == 1)
	{
		sdl->apply_text(178, 320, "FLASHLIGHT", header_font, sdl->return_screen());
		sdl->apply_text(400, 320, "COST:", header_font, sdl->return_screen());
		sdl->apply_text(500, 320, "$300", header_font, sdl->return_screen());
		sdl->apply_text(115, 350, "Flashlights let you see where minerals may be.", display_font, sdl->return_screen());
	}
	else if(store_selection->return_horiz() == 2 && store_selection->return_vert() == 1)
	{
		sdl->apply_text(178, 320, "HARD HAT", header_font, sdl->return_screen());
		sdl->apply_text(400, 320, "COST:", header_font, sdl->return_screen());
		sdl->apply_text(500, 320, "$200", header_font, sdl->return_screen());
		sdl->apply_text(170, 350, "Hard hats lessen the pain of cave-ins.", display_font, sdl->return_screen());	
	}
}

void Store_Objects::update_animated_graphics(SDL_Objects *sdl, PlayerData *player)
{
	// Update the background graphic.
	sdl->apply_surface(0, 0, store_graphic, sdl->return_screen());
	
	// Update the graphics for the items in the store.
	sdl->apply_surface(178, 24, shovel_graphic, sdl->return_screen());
	sdl->apply_surface(338, 24, pickaxe_graphic, sdl->return_screen());
	sdl->apply_surface(498, 24, bucket_graphic, sdl->return_screen());
	sdl->apply_surface(178, 152, dynamite_graphic, sdl->return_screen());
	sdl->apply_surface(338, 152, flashlight_graphic, sdl->return_screen());
	sdl->apply_surface(498, 152, hardhat_graphic, sdl->return_screen());
		
	// Update the EXIT text
	sdl->apply_colored_text(350, 270, 157, 157, 157, "EXIT", header_font, sdl->return_screen());
}

// Animate movement of the arrow.
void Store_Objects::animate_box_down(SDL_Objects *sdl, Selection_Arrow *selection, PlayerData *player)
{
	int temp = selection->return_arrow_y() - 128;

	while(temp != selection->return_arrow_y())
	{
		temp = temp + 32;
		update_animated_graphics(sdl, player);
		sdl->apply_surface(selection->return_arrow_x(), temp, selection_box, sdl->return_screen());
		SDL_Flip(sdl->return_screen());
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}

void Store_Objects::animate_box_up(SDL_Objects *sdl, Selection_Arrow *selection, PlayerData *player)
{
	int temp = selection->return_arrow_y() + 128;

	while(temp != selection->return_arrow_y())
	{
		temp = temp - 32;
		update_animated_graphics(sdl, player);
		sdl->apply_surface(selection->return_arrow_x(), temp, selection_box, sdl->return_screen());
		SDL_Flip(sdl->return_screen());
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}

void Store_Objects::animate_box_left(SDL_Objects *sdl, Selection_Arrow *selection, PlayerData *player)
{
	int temp = selection->return_arrow_x() + 160;

	while(temp != selection->return_arrow_x())
	{
		temp = temp - 40;
		update_animated_graphics(sdl, player);
		sdl->apply_surface(temp, selection->return_arrow_y(), selection_box, sdl->return_screen());
		SDL_Flip(sdl->return_screen());
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}

void Store_Objects::animate_box_right(SDL_Objects *sdl, Selection_Arrow *selection, PlayerData *player)
{
	int temp = selection->return_arrow_x() - 160;

	while(temp != selection->return_arrow_x())
	{
		temp = temp + 40;
		update_animated_graphics(sdl, player);
		sdl->apply_surface(temp, selection->return_arrow_y(), selection_box, sdl->return_screen());
		SDL_Flip(sdl->return_screen());
		SDL_Delay(sdl->MENU_ANIMATION_WAIT);
	}
}
