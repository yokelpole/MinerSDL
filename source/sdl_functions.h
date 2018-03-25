/*
 sdl_functions.h
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
 Class to deal with SDL.
 
 These large (!) files deal with screen updates, animations,
 mine graphics, etc.
 
 Also has a class to deal with the selection arrows used in the
 majority of screens in the town.
 */

#ifndef SDL
#define SDL

#include <string>

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"

#include "timer.h"

class PlayerData;
class MineData;

// Below enumeration allows to specify which direction is being travelled in the above animation function.
enum direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
    NONE        // Used if the screen is animating but the player is not moving.
};

// Class to hold data pertaining to SDL
class SDL_Objects
{
	private:
		SDL_Surface *screen;	// Main screen in program.
		
		SDL_Surface *hud_graphic;	// Holds the main graphic for the HUD.
			SDL_Surface *hud_shovel;
			SDL_Surface *hud_pickaxe;
			SDL_Surface *hud_bucket;
			SDL_Surface *hud_dynamite;
			SDL_Surface *hud_flashlight;
			SDL_Surface *hud_hardhat;
			SDL_Surface *hud_insurance;
		
		// Various graphics for the mine.
		SDL_Surface *dirt_graphic;
		SDL_Surface *elevator_graphic;
		SDL_Surface *mineshaft_graphic;
		SDL_Surface *granite_graphic;
		SDL_Surface *explored_graphic;
		SDL_Surface *hint_graphic;
		SDL_Surface *dynamite_graphic;
		SDL_Surface *diamond_graphic;		// Used for both HUD and mine.
		
		// Player graphics
		SDL_Surface *miner_graphic;
			SDL_Surface *miner_down_graphic_1;
			SDL_Surface *miner_down_graphic_2;
		SDL_Surface *miner_move_graphic;	// TEMPORARY
		
		// Boolean to handle miner animations
		bool miner_animate;
		
		// Mineral graphics
		SDL_Surface *platinum_graphic;
		SDL_Surface *gold_graphic;
		SDL_Surface *silver_graphic;
		SDL_Surface *coal_graphic;
		
		// Hazard graphics
		SDL_Surface *spring_graphic;
		SDL_Surface *water_graphic;
		SDL_Surface *cave_in_graphic;
		
		TTF_Font *status_font;		// Font used in display of user's health and money.
		
		TTF_Font *news_font;		// Font used in the HUD newsfeed.
		
		std::string player_status[8];	// Updates the player's status in the HUD.
		
		// Keeps tabs whether the player wants to quit the game or to menu.
		bool quitSDL;
		bool quit_to_menu;
				
	public:	
		SDL_Objects();		
		
		~SDL_Objects();

		// Function to update and display the HUD
		void display_hud(PlayerData *player);
		
		// Function to update the screen when in the mine.
		void update_mine_graphics(PlayerData *player, MineData *mine, direction way);
			void display_found_minerals(PlayerData *player, MineData *mine);
		void animate_mine_graphics(PlayerData *player, MineData *mine, direction way);
			void display_background_layer(PlayerData *player, MineData *mine, direction way, bool animate_vert, bool animate_horiz,int mine_x, int mine_y);
			void display_sprite_layer(PlayerData *player, MineData *mine, direction way, bool animate_vert, bool animate_horiz, int mine_x, int mine_y);
			void display_found_minerals_animated(PlayerData *player, MineData *mine, direction way, bool animate_vert, bool animate_horiz, int mine_x, int mine_y);
		
		// Function to tell which animation graphic to use
		bool which_animation();
		
		// Updates the text in the HUD and clears out older information.
		void update_status_text(std::string new_text);
		
		// Provides the text within player_status
		std::string return_status_text(int array);
		
		// Clear out the text in the HUD.
		void clear_status_text();
		
		// Function to set a pointer to *screen
		void set_screen(SDL_Surface *screen);
		
		// Return the location of *screen
		SDL_Surface *return_screen();
		
		// Allows an instance of SDL_Surface to be applied to another surface.
		void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination);
		
		// Allows a line of text via SDL_ttf to be applied to a . Used in store.
		void apply_text(int x, int y, std::string input_string, TTF_Font *font, SDL_Surface *destination);
		void apply_colored_text(int x, int y, int r, int g, int b, std::string input_string, TTF_Font *font, SDL_Surface *destination);
		
		// Closes out SDL
		void quit_sdl();
		void set_quitSDL();
		bool return_quitSDL();
		
		// Quit to the main menu of the game.
		void set_quit_to_menu(bool value);
		bool return_quit_to_menu();
		
		// Values for the timers in the SDL program.
		int SDL_WAIT;
		int KEYPRESS_WAIT;
		int ENTER_WAIT;
		int MENU_ANIMATION_WAIT;		
		int MINE_ANIMATION_WAIT;
};

// Provides a class for the selection arrow that appears on the town and shop screens.
class Selection_Arrow
{
	private:
		// Stores the location of the arrow in x and y
		int arrow_x;
		int arrow_y;
		
		// Stores the starting location of x and y
		int arrow_start_x;
		int arrow_start_y;
	
		// Stores how much the arrow is to move with each movement.
		int amount_x;
		int amount_y;
		
		// Stores how many moves are available up and down.
		int available_horiz;
		int available_vert;
		
		// Keeps the current amount of moves made either up or down.
		int location_horiz;
		int location_vert;
	public:
		Selection_Arrow(int avail_horiz, int avail_vert, int amnt_x, int amnt_y);
		
		// Set the default location for the arrow
		void set_arrow_initial(int x, int y);
		
		// The following check and return whether the arrow is able to move up/down.
		bool move_up();
		bool move_down();
		bool move_left();
		bool move_right();
		
		// The following return the value that the arrow is currently located at.
		int return_horiz();
		int return_vert();
		
		// The following returns the amount of movement that is to happen with each event.
		int return_movement_x();
		int return_movement_y();
		
		// The following returns the default x and y of the arrow
		int return_arrow_x();
		int return_arrow_y();
};
#endif 
