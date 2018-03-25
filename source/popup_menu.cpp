/* 
 popup_menu.cpp
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
 Menu that pops up when the player hits escape.
 Allows for settings to be changed, game to be loaded/saved.
 Also has popup screens to confirm player quitting, retrying, etc.
*/

// General SDL includes.
#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

// General program includes
#include "sdl_functions.h"
#include "classes.h"
#include "popup_menu.h"
#include "save_load.h"
#include "instructions.h"

// Display the general options menu for the town screen.
void display_popup_menu(SDL_Objects *sdl, MineData *mine, PlayerData *player)
{
	// Initialize the objects for the popup menu.
	Popup_Menu menu;
	
	// Initialize the selection arrow
	Selection_Arrow selection(1, 6, 0, 38);		// Initializes the arrow object with six places to go up/down.
	selection.set_arrow_initial(260, 70);
	
	// Initialize an event to track the user's input
	SDL_Event user_input;
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
	
	// Apply the graphics on screen and update them.
	menu.update_popup_menu(sdl, &selection);
	sdl->display_hud(player);
	SDL_Flip(sdl->return_screen());
	
	// Main loop for the screen
	while(!exit)
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
				menu.animate_arrow_down(sdl, &selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_UP])
		{
			if(selection.move_up())
			{
				menu.animate_arrow_up(sdl, &selection);
			}
			update_screen = true;
		}
		else if(keystate[SDLK_ESCAPE])
		{
			// Allows the player to close the popup menu by hitting escape.
			exit = true;
		}		
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER] || keystate[SDLK_BACKSPACE])
		{
			if(selection.return_vert() == 0)
			{	
				// Allow the player to leave the popup menu and return to the game.
				exit = true;
			}
			else if(selection.return_vert() == 1)
			{
				// Save game.
				SDL_Delay(sdl->KEYPRESS_WAIT);
				save_game(mine, player);
				sdl->update_status_text("Game saved!");
				update_screen = true;
			}
			else if(selection.return_vert() == 2)
			{
				// Load game.
				SDL_Delay(sdl->KEYPRESS_WAIT);
				load_game(mine, player);
				sdl->clear_status_text();
				sdl->update_status_text("Game loaded!");
				update_screen = true;
			}
            else if(selection.return_vert() == 3)
            {
                // Display the instructions.
                display_instructions(sdl);
                update_screen = true;
            }
			else if(selection.return_vert() == 4)
			{
				// Quit to the main menu.
				sdl->set_quit_to_menu(true);
				exit = true;
			}
			else if(selection.return_vert() == 5)
			{
				// Quit to the operating system.
				sdl->set_quitSDL();
				exit = true;
			}
		}
					
		if(update_screen)
		{
			// Apply the graphics on screen and update them.
			menu.update_popup_menu(sdl, &selection);	
			sdl->display_hud(player);
			SDL_Flip(sdl->return_screen());
		}
		
		SDL_Delay(sdl->SDL_WAIT);	
	}
	
	SDL_Delay(sdl->ENTER_WAIT);
}

// Display a message telling the player they have died.
void display_dead_message(SDL_Objects *sdl)
{
	// Initialize the objects for the popup menu.
	Popup_Menu menu;
	
	// Initialize the selection arrow
	Selection_Arrow selection(1, 1, 0, 38);		// Initializes the arrow object with six places to go up/down.
	selection.set_arrow_initial(330, 247);
	
	// Apply the graphics on screen and update them.
	menu.update_player_dead_message(sdl, &selection);	
	SDL_Flip(sdl->return_screen());	
	
	// Wait for user input.
	menu.wait_for_keypress(sdl);
		
	SDL_Delay(sdl->ENTER_WAIT);
}

// Display a message telling the player they've gone broke.
void display_broke_message(SDL_Objects *sdl)
{
	// Initialize the objects for the popup menu.
	Popup_Menu menu;
	
	// Initialize the selection arrow
	Selection_Arrow selection(1, 1, 0, 38);		// Initializes the arrow object with six places to go up/down.
	selection.set_arrow_initial(330, 247);
	
	// Apply the graphics on screen and update them.
	menu.update_player_broke_message(sdl, &selection);	
	SDL_Flip(sdl->return_screen());	
	
	// Wait for user input.
	menu.wait_for_keypress(sdl);
		
	SDL_Delay(sdl->ENTER_WAIT);
}

// Displays a menu allowing the user to confirm quit to menu.
void display_confirm_quit(SDL_Objects *sdl)
{
	// Initialize the objects for the popup menu.
	Popup_Menu menu;
	
	// Initialize the selection arrow
	Selection_Arrow selection(1, 2, 0, 40);
	selection.set_arrow_initial(315, 210);
	
	// Initialize an event to track the user's input
	SDL_Event user_input;
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
		
	// Apply the graphics on the screen
	menu.update_player_confirm_quit(sdl, &selection);
	SDL_Flip(sdl->return_screen());
	
	while(!exit)
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
			selection.move_down();
			update_screen = true;
			SDL_Delay(sdl->KEYPRESS_WAIT);
		}
		else if(keystate[SDLK_UP])
		{
			selection.move_up();
			update_screen = true;
			SDL_Delay(sdl->KEYPRESS_WAIT);
		}
		else if(keystate[SDLK_ESCAPE] || keystate[SDLK_BACKSPACE])
		{
			// Allows the player to close the popup menu by hitting escape.
			exit = true;
			update_screen = true;
			SDL_Delay(sdl->KEYPRESS_WAIT);
		}		
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER])
		{
			if(selection.return_vert() == 0)
			{			
				// User has selected no
			}
			else if(selection.return_vert() == 1)
			{
				// User has selected yes
				sdl->set_quit_to_menu(true);
				SDL_Delay(sdl->KEYPRESS_WAIT);
				return;
			}
			
			update_screen = true;
			exit = true;
		}
		
		if(update_screen)
		{
			menu.update_player_confirm_quit(sdl, &selection);
			SDL_Flip(sdl->return_screen());
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}
}

// Allow the player to spend all of their money on healthcare.
bool display_confirm_spend(SDL_Objects *sdl)
{
	// Initialize the objects for the popup menu.
	Popup_Menu menu;
	
	// Initialize the selection arrow
	Selection_Arrow selection(1, 2, 0, 40);
	selection.set_arrow_initial(315, 210);
	
	// Initialize an event to track the user's input
	SDL_Event user_input;
	
	// Initialize timer for keypress repeating.
	Timer keyboard_timer;
	bool exit = false;		// Keeps track of whether to exit main loop.
	bool update_screen = false;	// Keeps track of when to update the screen.
		
	// Apply the graphics on the screen
	menu.update_player_confirm_spend(sdl, &selection);
	SDL_Flip(sdl->return_screen());
	
	while(!exit)
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
			selection.move_down();
			update_screen = true;
			SDL_Delay(sdl->KEYPRESS_WAIT);
		}
		else if(keystate[SDLK_UP])
		{
			selection.move_up();
			update_screen = true;
			SDL_Delay(sdl->KEYPRESS_WAIT);
		}
		else if(keystate[SDLK_ESCAPE] || keystate[SDLK_BACKSPACE])
		{
			// Allows the player to close the popup menu by hitting escape.
			exit = true;
			update_screen = true;
		}		
		else if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER])
		{
			if(selection.return_vert() == 0)
			{			
				// User has selected no
				SDL_Delay(sdl->KEYPRESS_WAIT);
				return false;
			}
			else if(selection.return_vert() == 1)
			{
				// User has selected yes
				SDL_Delay(sdl->KEYPRESS_WAIT);
				return true;
			}
			
			update_screen = true;
			exit = true;
		}
		
		if(update_screen)
		{
			menu.update_player_confirm_spend(sdl, &selection);
			SDL_Flip(sdl->return_screen());
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}	
}

Popup_Menu::Popup_Menu()
{
	menu_backdrop = IMG_Load("./Graphics/popup_menu/popup_menu.png");
    background = IMG_Load("./Graphics/start_screen/background.png");

	confirmation_menu = IMG_Load("./Graphics/popup_menu/confirmation_menu.png");
	
	menu_arrow = IMG_Load("./Graphics/popup_menu/arrow.png");
	
	headstone_graphic = IMG_Load("./Graphics/popup_menu/headstone.png");
	broke_graphic = IMG_Load("./Graphics/popup_menu/broke.png");
	
	font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
	small_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 20);
}

Popup_Menu::~Popup_Menu()
{
	SDL_FreeSurface(menu_backdrop);
    SDL_FreeSurface(background);
	SDL_FreeSurface(confirmation_menu);
	
	SDL_FreeSurface(menu_arrow);
	
	SDL_FreeSurface(headstone_graphic);
	SDL_FreeSurface(broke_graphic);
	
	TTF_CloseFont(font);
}

// Update the graphics for the main popup menu.
void Popup_Menu::update_popup_menu(SDL_Objects *sdl, Selection_Arrow *selection)
{
    sdl->apply_surface(0, 0, background, sdl->return_screen());    
	sdl->apply_surface(234, 40, menu_backdrop, sdl->return_screen());
	
	sdl->apply_text(300, 70, "RESUME", font, sdl->return_screen());
	sdl->apply_text(300, 108, "SAVE GAME", font, sdl->return_screen());
	sdl->apply_text(300, 146, "LOAD GAME", font, sdl->return_screen());
    sdl->apply_text(300, 184, "HELP", font, sdl->return_screen());
	sdl->apply_text(300, 220, "QUIT (MENU)", font, sdl->return_screen());
	sdl->apply_text(300, 256, "QUIT TO OS", font, sdl->return_screen());
	
	sdl->apply_surface(selection->return_arrow_x(), selection->return_arrow_y(), menu_arrow, sdl->return_screen());
}

// Display the 'You have died' menu on the screen.
void Popup_Menu::update_player_dead_message(SDL_Objects *sdl, Selection_Arrow *selection)
{
	sdl->apply_surface(234, 67, confirmation_menu, sdl->return_screen());
	
	sdl->apply_text(285, 99, "You're dead!", font, sdl->return_screen());
	
	sdl->apply_surface(260, 142, headstone_graphic, sdl->return_screen());
	
	sdl->apply_text(360, 150, "You died", small_font, sdl->return_screen());
	sdl->apply_text(360, 175, "with a tool", small_font, sdl->return_screen());
	sdl->apply_text(360, 200, "in your hand.", small_font, sdl->return_screen());
	sdl->apply_text(365, 245, "OK", font, sdl->return_screen());
	
	sdl->apply_surface(selection->return_arrow_x(), selection->return_arrow_y(), menu_arrow, sdl->return_screen());
}

// Display the 'You are dead' menu on the screen.
void Popup_Menu::update_player_broke_message(SDL_Objects *sdl, Selection_Arrow *selection)
{
	sdl->apply_surface(234, 67, confirmation_menu, sdl->return_screen());
	
	sdl->apply_text(280, 99, "You're broke!", font, sdl->return_screen());
	
	sdl->apply_surface(260, 142, broke_graphic, sdl->return_screen());
	
	sdl->apply_text(365, 160, "No money", small_font, sdl->return_screen());
	sdl->apply_text(365, 185, "no honey.", small_font, sdl->return_screen());
	
	sdl->apply_text(365, 245, "OK", font, sdl->return_screen());
	
	sdl->apply_surface(selection->return_arrow_x(), selection->return_arrow_y(), menu_arrow, sdl->return_screen());
}

// Display confirmation menu for player to quit current game.
void Popup_Menu::update_player_confirm_quit(SDL_Objects *sdl, Selection_Arrow *selection)
{
	sdl->apply_surface(234, 67, confirmation_menu, sdl->return_screen());
	
	sdl->apply_text(288, 99, "Really quit?", font, sdl->return_screen());
	
	sdl->apply_text(350, 210, "NO", font, sdl->return_screen());
	sdl->apply_text(350, 250, "YES", font, sdl->return_screen());
	
	sdl->apply_surface(selection->return_arrow_x(), selection->return_arrow_y(), menu_arrow, sdl->return_screen());
}

// Display confirmation for player that is going to spend all of
// their money healing at the hospital.
void Popup_Menu::update_player_confirm_spend(SDL_Objects *sdl, Selection_Arrow *selection)
{
	sdl->apply_surface(234, 67, confirmation_menu, sdl->return_screen());
	
	sdl->apply_text(288, 99, "WARNING!", font, sdl->return_screen());
	
	sdl->apply_text(275, 125, "Spend ALL of", font, sdl->return_screen());
	sdl->apply_text(275, 150, "your money!", font, sdl->return_screen());
	
	sdl->apply_text(350, 210, "NO", font, sdl->return_screen());
	sdl->apply_text(350, 250, "YES", font, sdl->return_screen());
	
	sdl->apply_surface(selection->return_arrow_x(), selection->return_arrow_y(), menu_arrow, sdl->return_screen());
}

// Animate the movement of the arrow.
void Popup_Menu::animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() + 38;

	while(temp != selection->return_arrow_y())
	{
		temp = temp - 19;
		sdl->apply_surface(234, 40, menu_backdrop, sdl->return_screen());
        
        sdl->apply_text(300, 70, "RESUME", font, sdl->return_screen());
        sdl->apply_text(300, 108, "SAVE GAME", font, sdl->return_screen());
        sdl->apply_text(300, 146, "LOAD GAME", font, sdl->return_screen());
        sdl->apply_text(300, 184, "HELP", font, sdl->return_screen());
        sdl->apply_text(300, 220, "QUIT (MENU)", font, sdl->return_screen());
        sdl->apply_text(300, 256, "QUIT TO OS", font, sdl->return_screen());   
        
		sdl->apply_surface(selection->return_arrow_x(), temp, menu_arrow, sdl->return_screen());
		SDL_UpdateRect(sdl->return_screen(), 234, 40, 60, 270);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT * 2);
	}		
}

void Popup_Menu::animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection)
{
	int temp = selection->return_arrow_y() - 38;

	while(temp != selection->return_arrow_y())
	{
		temp = temp + 19;
		sdl->apply_surface(234, 40, menu_backdrop, sdl->return_screen());
        
        sdl->apply_text(300, 70, "RESUME", font, sdl->return_screen());
        sdl->apply_text(300, 108, "SAVE GAME", font, sdl->return_screen());
        sdl->apply_text(300, 146, "LOAD GAME", font, sdl->return_screen());
        sdl->apply_text(300, 184, "HELP", font, sdl->return_screen());
        sdl->apply_text(300, 220, "QUIT (MENU)", font, sdl->return_screen());
        sdl->apply_text(300, 256, "QUIT TO OS", font, sdl->return_screen());
        
		sdl->apply_surface(selection->return_arrow_x(), temp, menu_arrow, sdl->return_screen());
		SDL_UpdateRect(sdl->return_screen(), 234, 40, 60, 270);
		SDL_Delay(sdl->MENU_ANIMATION_WAIT * 2);
	}
}

// Freezes the screen until the player presses enter or esc.
void Popup_Menu::wait_for_keypress(SDL_Objects *sdl)
{
	// Initialize an event to track the user's input
	SDL_Event user_input;
	Timer keyboard_timer;
	bool exit = false;
	
	// Main loop for the screen
	while(!exit)
	{
		Uint8 *keystate = SDL_GetKeyState(NULL);	// Captures keyboard input.
		
		while(SDL_PollEvent( &user_input ))
		{	
			// Quit if the user chooses to close the window.
			if(user_input.type == SDL_QUIT)
			{
				sdl->set_quitSDL();
				exit = true;
			}
			else if(user_input.type == SDL_KEYDOWN)
			{
				keyboard_timer.begin_timer();
			}
			else if(user_input.type == SDL_KEYUP)
			{
				keyboard_timer.stop_timer();
			}
		}
		
		// Respond to the user's key presses
		if(keystate[SDLK_RETURN] || keystate[SDLK_KP_ENTER] || keystate[SDLK_ESCAPE])
		{
			exit = true;
		}
		
		SDL_Delay(sdl->SDL_WAIT);
	}
}
