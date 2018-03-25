/*
 popup_menu.h
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
 --------------------------------------
 Menu that pops up when the player hits escape.
 Allows for settings to be changed, game to be loaded/saved.
 Also has popup screens to confirm player quitting, retrying, etc.
*/

#ifndef POPUP_MENU
#define POPUP_MENU

// Display the popup menu that appears when the player presses escape.
void display_popup_menu(SDL_Objects *sdl, MineData *mine, PlayerData *player);
void display_dead_message(SDL_Objects *sdl);
void display_broke_message(SDL_Objects *sdl);
void display_confirm_quit(SDL_Objects *sdl);
bool display_confirm_spend(SDL_Objects *sdl);

class Popup_Menu
{
	private:
		SDL_Surface *menu_backdrop;
        SDL_Surface *background;
		SDL_Surface *confirmation_menu;
				
		SDL_Surface *menu_arrow;
		
		SDL_Surface *headstone_graphic;
		SDL_Surface *broke_graphic;
		
		TTF_Font *font;
		TTF_Font *small_font;
		
	public:
		Popup_Menu();
		
		~Popup_Menu();
		
		// Display the popup menu on screen.
		void update_popup_menu(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Display the 'confirm exit to menu' menu on the screen.
		void update_confirm_exit(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Display the 'You have died' menu on the screen.
		void update_player_dead_message(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Display the 'You are broke' menu on the screen.
		void update_player_broke_message(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Display confirmation for the player to quit current game.
		void update_player_confirm_quit(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Display confirmation for player that is going to spend all of
		// their money healing at the hospital.
		void update_player_confirm_spend(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Animate the movement of the arrow.
		void animate_arrow_up(SDL_Objects *sdl, Selection_Arrow *selection);
		void animate_arrow_down(SDL_Objects *sdl, Selection_Arrow *selection);
		
		// Waits for user input.
		void wait_for_keypress(SDL_Objects *sdl);
};

#endif
