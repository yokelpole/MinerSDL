/*
 sdl_functions.cpp
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

#include <string>
#include <sstream> 		// Allows for easy conversion of int into string for HUD.
#include <iostream>

#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

#include "sdl_functions.h"
#include "classes.h"
#include "timer.h"

// Initialize SDL_Objects
SDL_Objects::SDL_Objects()
{	
	// Initialize display screen.
	// Currently resolution is restricted to 768x480. 16:10
	screen = SDL_SetVideoMode(768, 480, 32, SDL_HWSURFACE);
	
	// Initialize TTF_Font for display of TrueType fonts.
	TTF_Init();
	
	// Load the HUD, as it will be needed by most screens.
	hud_graphic = IMG_Load("./Graphics/hud/hud.png");

	// Load the inventory graphics used by the HUD, as this is needed as well.
	hud_shovel = IMG_Load("./Graphics/hud/shovel.png");
	hud_pickaxe = IMG_Load("./Graphics/hud/pickaxe.png");
	hud_bucket = IMG_Load("./Graphics/hud/bucket.png");
	hud_dynamite = IMG_Load("./Graphics/hud/tnt.png");
	hud_flashlight = IMG_Load("./Graphics/hud/flashlight.png");
	hud_hardhat = IMG_Load("./Graphics/hud/hardhat.png");
	hud_insurance = IMG_Load("./Graphics/hud/red_cross.png");
	
	// Load the graphics to be used in the mine.
	dirt_graphic = IMG_Load("./Graphics/mine/dirt.png");
	elevator_graphic = IMG_Load("./Graphics/mine/elevator.png");	
	mineshaft_graphic = IMG_Load("./Graphics/mine/shaft.png");
	
	miner_graphic = IMG_Load("./Graphics/mine/miner.png");
	miner_down_graphic_1 = IMG_Load("./Graphics/mine/miner-down-1.png");
	miner_down_graphic_2 = IMG_Load("./Graphics/mine/miner-down-2.png");
	
	miner_move_graphic = IMG_Load("./Graphics/mine/minermove.png");
	
	miner_animate = false;
	
	granite_graphic = IMG_Load("./Graphics/mine/granite.png");
	explored_graphic = IMG_Load("./Graphics/mine/explored.png");
	hint_graphic = IMG_Load("./Graphics/mine/hint.png");
	dynamite_graphic = IMG_Load("./Graphics/mine/dynamite.png");
	diamond_graphic = IMG_Load("./Graphics/mine/diamond.png");
	
	platinum_graphic = IMG_Load("./Graphics/mine/platinum.png");
	gold_graphic = IMG_Load("./Graphics/mine/gold.png");
	silver_graphic = IMG_Load("./Graphics/mine/silver.png");
	coal_graphic = IMG_Load("./Graphics/mine/coal.png");
	
	spring_graphic = IMG_Load("./Graphics/mine/spring.png");
	water_graphic = IMG_Load("./Graphics/mine/water.png");
	cave_in_graphic = IMG_Load("./Graphics/mine/cave-in.png");

	// Load the fonts for the above graphic.
	status_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 28);
	news_font = TTF_OpenFont("./Fonts/DejaVuSans-Bold.ttf", 12);
	
	// Set quitSDL to false.
	quitSDL = false;
	quit_to_menu = true;	// Set to true to start with startup screen.
	
	SDL_WAIT = 10;
	KEYPRESS_WAIT = 125;
	ENTER_WAIT = 175;
	MINE_ANIMATION_WAIT = 60;
	MENU_ANIMATION_WAIT = 20;	
}

SDL_Objects::~SDL_Objects()
{
	SDL_FreeSurface(screen);
	
	// Free the HUD graphics.
	SDL_FreeSurface(hud_graphic);
	SDL_FreeSurface(hud_shovel);
	SDL_FreeSurface(hud_pickaxe);
	SDL_FreeSurface(hud_bucket);
	SDL_FreeSurface(hud_dynamite);
	SDL_FreeSurface(hud_flashlight);
	SDL_FreeSurface(hud_hardhat);
	SDL_FreeSurface(hud_insurance);
	
	// Free the graphics used in the mine.
	SDL_FreeSurface(dirt_graphic);
	
	SDL_FreeSurface(elevator_graphic);
	SDL_FreeSurface(mineshaft_graphic);	
	
	SDL_FreeSurface(miner_graphic);
	SDL_FreeSurface(miner_down_graphic_1);
	SDL_FreeSurface(miner_down_graphic_2);
	SDL_FreeSurface(miner_move_graphic);
	
	SDL_FreeSurface(granite_graphic);
	SDL_FreeSurface(explored_graphic);
	SDL_FreeSurface(hint_graphic);
	SDL_FreeSurface(dynamite_graphic);
	SDL_FreeSurface(diamond_graphic);
	
	SDL_FreeSurface(platinum_graphic);
	SDL_FreeSurface(gold_graphic);
	SDL_FreeSurface(silver_graphic);
	SDL_FreeSurface(coal_graphic);
	
	SDL_FreeSurface(spring_graphic);
	SDL_FreeSurface(water_graphic);
	SDL_FreeSurface(cave_in_graphic);
	
	// Close out the fonts used in the HUD
	TTF_CloseFont(status_font);
	TTF_CloseFont(news_font);
	TTF_Quit();
}

// Displays the HUD
void SDL_Objects::display_hud(PlayerData *player)
{
	std::string temp_string;	// Used as a buffer for numerical values (int)
	std::stringstream temp_stringstream;	// Used to convert the int to health string
	
	SDL_Rect hud_location;		// Stores where on the screen the HUD will be located.
		hud_location.x = 0;
		hud_location.y = 384;
		hud_location.w = 720;
		hud_location.h = 72;
	
	SDL_Rect hud_text;
		hud_text.x = 250;
		hud_text.y = 408;
		hud_text.w = 550;
		hud_text.h = 72;
		
	apply_surface(hud_location.x, hud_location.y, hud_graphic, return_screen());

	// Below applies to applying status text to the HUD.
	// Should put this into its own function.
	apply_text((hud_location.x + 4), (hud_location.y + 2), "Health:", status_font, return_screen());
	
	// Apply the numerical value of the player's health to the HUD.
	// Int has to be converted to a string before this can be done...
	// Check to see if the text has to be displayed in red or white.
	temp_stringstream << player->get_health();
	temp_string = temp_stringstream.str();
	if(player->get_health() > 25)
	{
		apply_text((hud_location.x + 122), (hud_location.y + 2), temp_string.c_str(), status_font, return_screen());
	}
	else
	{
		apply_colored_text((hud_location.x + 122), (hud_location.y + 2), 255, 0, 0, temp_string.c_str(), status_font, return_screen());
	}
	temp_stringstream.str("");
	
	apply_text((hud_location.x + 4), (hud_location.y + 34), "Money:", status_font, return_screen());
	
	temp_stringstream << player->get_money();
	temp_string = temp_stringstream.str();
	if(player->get_money() > 150)
	{
		apply_text((hud_location.x + 122), (hud_location.y + 34), temp_string.c_str(), status_font, return_screen());
	}
	else
	{
		apply_colored_text((hud_location.x + 122), (hud_location.y + 34), 255, 0, 0, temp_string.c_str(), status_font, return_screen());
	}
	temp_stringstream.str("");

	// Display the player's mineral amounts
	temp_stringstream << player->get_platinum();
	temp_string = temp_stringstream.str();
	apply_text((hud_location.x + 26), (hud_location.y + 80), temp_string.c_str(), news_font, return_screen());
	temp_stringstream.str("");

	temp_stringstream << player->get_gold();
	temp_string = temp_stringstream.str();
	apply_text((hud_location.x + 86), (hud_location.y + 80), temp_string.c_str(), news_font, return_screen());
	temp_stringstream.str("");
	
	temp_stringstream << player->get_silver();
	temp_string = temp_stringstream.str();
	apply_text((hud_location.x + 146), (hud_location.y + 80), temp_string.c_str(), news_font, return_screen());
	temp_stringstream.str("");
	
	temp_stringstream << player->get_coal();
	temp_string = temp_stringstream.str();
	apply_text((hud_location.x + 206), (hud_location.y + 80), temp_string.c_str(), news_font, return_screen());
	temp_stringstream.str("");
	
	// Display the text buffer for the update screen.
	for(int x = 0; x < 7; x++)
	{
		temp_string = return_status_text(x);
		apply_text((hud_location.x + 250), (hud_location.y + (x * 15)), temp_string.c_str(), news_font, return_screen());
	}
	
	// Display whether the player has insurance or not.
	if(player->get_has_insurance())
	{
		apply_surface((hud_location.x + 200), hud_location.y, hud_insurance, return_screen());
	}
	
	// Check the player's inventory and update the screen accordingly
	if(player->get_has_shovel())
	{
		apply_surface((hud_location.x + 576), hud_location.y, hud_shovel, return_screen());
	}
	if(player->get_has_axe())
	{
		apply_surface((hud_location.x + 624), hud_location.y, hud_pickaxe, return_screen());
	}
	if(player->get_has_bucket())
	{
		apply_surface((hud_location.x + 672), hud_location.y, hud_bucket, return_screen());
	}
	if(player->get_has_dynamite())
	{
		apply_surface((hud_location.x + 720), hud_location.y, hud_dynamite, return_screen());
	}
	if(player->get_has_flashlight())
	{
		apply_surface((hud_location.x + 576), (hud_location.y + 48), hud_flashlight, return_screen());
	}
	if(player->get_has_hardhat())
	{
		apply_surface((hud_location.x + 624), (hud_location.y + 48), hud_hardhat, return_screen());
	}
	if(player->get_has_diamond())
	{
		apply_surface((hud_location.x + 720), (hud_location.y + 48), diamond_graphic, return_screen());
	}
}

// Function to update the screen when in the mine.
void SDL_Objects::update_mine_graphics(PlayerData *player, MineData *mine, direction way)
{
    if(way != NONE)
    {
        animate_mine_graphics(player, mine, way);
    }
    else
    {
        // Variables used for temporary storage of where the centre of the screen
        // should be.
        int mine_x = 0;
        int mine_y = 0;
	
        // Determine if the playing area should be focused on the player or not.
        if(player->get_location_x() > 8 && player->get_location_x() < (mine->get_map_x() - 10))
        {
            mine_x = player->get_location_x() - 7;
        }
        else if(player->get_location_x() >= (mine->get_map_x() - 10))
        {
		mine_x = (mine->get_map_x() - 16);
        }
	
        if(player->get_location_y() > 5 && player->get_location_y() < (mine->get_map_y() - 4))
        {
            mine_y = player->get_location_y() - 4;
        }
        else if(player->get_location_y() >= (mine->get_map_y() - 4))
        {
            mine_y = (mine->get_map_y() - 8);
        }
		
        // Show the area of the screen that is to be shown.
        // Below variables store position on the 48x48 grid.
        int y_tile_position = 0;
        int x_tile_position = 0;
	
        for(int y = mine_y; y < (mine_y + 8); y++)
        {
            for(int x = mine_x; x < (mine_x + 16); x++)
            {
                // Apply the appropriate graphic for the location.
                if(mine->get_explored(x,y) == false)
                {
                    // Apply everything as dirt if the player has no flashlight.
                    if(player->get_has_flashlight() == false || mine->return_recently_found_countdown() > 0)
                    {
                        apply_surface(x_tile_position, y_tile_position, dirt_graphic, return_screen());
                    }
                    // Otherwise occasionally hint where minerals are to the player.
                    else
                    {
                        if((y + 2 > player->get_location_y()) && (y - 2 < player->get_location_y())
                           && ((x + 2 > player->get_location_x()) && (x -2 < player->get_location_x()))
                           && mine->return_recently_found_material() == NOTHING
                           && ((mine->get_contents(x,y) == GRANITE)
                               || (mine->get_contents(x,y) == SPRING)
                               || (mine->get_contents(x,y) == CAVE_IN)
                               || (mine->get_contents(x,y) == COAL)
                               || (mine->get_contents(x,y) == SILVER)
                               || (mine->get_contents(x,y) == GOLD)
                               || (mine->get_contents(x,y) == PLATINUM
                                   || (mine->get_contents(x,y) == DIAMOND))))
                        {
                            int random_number = 0;
						
                            random_number = rand() % 6;
                            if(random_number == 5)
                            {
                                apply_surface(x_tile_position, y_tile_position, hint_graphic, return_screen());
                            }
                            else
                            {
                                apply_surface(x_tile_position, y_tile_position, dirt_graphic, return_screen());
                            }
                        }
                        else
                        {
                            apply_surface(x_tile_position, y_tile_position, dirt_graphic, return_screen());
                        }
                    }
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) != ELEVATOR 
                        && mine->get_contents(x,y) != SHAFT
                        && mine->get_contents(x,y) != GRANITE
                        && mine->get_contents(x,y) != SPRING
                        && mine->get_contents(x,y) != WATER
                        && mine->get_contents(x,y) != CAVE_IN
                        && mine->get_contents(x,y) != COAL
                        && mine->get_contents(x,y) != SILVER
                        && mine->get_contents(x,y) != GOLD
                        && mine->get_contents(x,y) != PLATINUM
                        && mine->get_contents(x,y) != DYNAMITE
                        && mine->get_contents(x,y) != DIAMOND)
                {
                    apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
                }		
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == ELEVATOR)
                {
                    apply_surface(x_tile_position, y_tile_position, elevator_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == SHAFT)
                {
                    apply_surface(x_tile_position, y_tile_position, mineshaft_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == GRANITE)
                {
                    apply_surface(x_tile_position, y_tile_position, granite_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == CAVE_IN)
                {
                    apply_surface(x_tile_position, y_tile_position, cave_in_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == SPRING)
                {
                    apply_surface(x_tile_position, y_tile_position, spring_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == WATER)
                {
                    apply_surface(x_tile_position, y_tile_position, water_graphic, return_screen());
                }
                // The below else ifs come into effect if dynamite has uncovered minerals.
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == COAL)
                {
                    apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
                    apply_surface(x_tile_position, y_tile_position, coal_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == SILVER)
                {
                    apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
                    apply_surface(x_tile_position, y_tile_position, silver_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == GOLD)
                {
                    apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
                    apply_surface(x_tile_position, y_tile_position, gold_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == PLATINUM)
                {
                    apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
                    apply_surface(x_tile_position, y_tile_position, platinum_graphic, return_screen());
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == DYNAMITE)
                {
                    apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
                    apply_surface(x_tile_position, y_tile_position, dynamite_graphic, return_screen());				
                }
                else if(mine->get_explored(x,y) == true
                        && mine->get_contents(x,y) == DIAMOND)
                {
                    apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
                    apply_surface(x_tile_position, y_tile_position, diamond_graphic, return_screen());						
                }
			
                // Applies the little miner dude on the screen.
                if(x == player->get_location_x() && y == player->get_location_y())
                {
                    apply_surface(x_tile_position, y_tile_position, miner_graphic, return_screen());
                }
			
                x_tile_position = x_tile_position + 48;
            }
            x_tile_position = 0;
            y_tile_position = y_tile_position + 48;
        }
        
        // Only display found minerals if the timer is higher than zero.
        if(mine->return_recently_found_material() != NOTHING)
        {
            display_found_minerals(player, mine);
        }
        
    }
}

// Displays found minerals when the screen is stationary.
void SDL_Objects::display_found_minerals(PlayerData *player, MineData *mine)
{
	// Variables used for temporary storage of where the centre of the screen
	// should be.
	int mine_x = 0;
	int mine_y = 0;
	
	// Determine if the playing area should be focused on the player or not.
	if(player->get_location_x() > 8 && player->get_location_x() < (mine->get_map_x() - 10))
	{
		mine_x = player->get_location_x() - 7;
	}
	else if(player->get_location_x() >= (mine->get_map_x() - 10))
	{
		mine_x = (mine->get_map_x() - 16);
	}
	
	if(player->get_location_y() > 5 && player->get_location_y() < (mine->get_map_y() - 4))
	{
		mine_y = player->get_location_y() - 4;
	}
	else if(player->get_location_y() >= (mine->get_map_y() - 4))
	{
		mine_y = (mine->get_map_y() - 8);
	}
		
	// Show the area of the screen that is to be shown.
	// Below variables store position on the 48x48 grid.
	int y_tile_position = 0;
	int x_tile_position = 0;	

	for(int y = mine_y; y < (mine_y + 11); y++)
	{
		for(int x = mine_x; x < (mine_x + 17); x++)
		{	
			if(x == mine->return_recently_found_x() && y == mine->return_recently_found_y())
			{
				if(mine->return_recently_found_material() == COAL)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), coal_graphic, return_screen());
				}
				else if(mine->return_recently_found_material() == SILVER)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), silver_graphic, return_screen());
				}
				else if(mine->return_recently_found_material() == GOLD)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), gold_graphic, return_screen());
				}
				else if(mine->return_recently_found_material() == PLATINUM)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), platinum_graphic, return_screen());
				}
				else if(mine->return_recently_found_material() == DIAMOND)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), diamond_graphic, return_screen());
				}				
			}
			x_tile_position += 48;
		}
	
		x_tile_position = 0;
		y_tile_position += 48;
	}
}	

// Following four functions allow for tile to tile animation to occur within the mine.
void SDL_Objects::animate_mine_graphics(PlayerData *player, MineData *mine, direction way)
{
	// Determine whether the graphics will be animated.
	// Determine if the playing area should be focused on the player or not.
	int mine_x = 0;
	int mine_y = 0;
	
	// Booleans to determine whether movement should be animated.
	bool animate_vert = false;
	bool animate_horiz = false;
	
	if((player->get_location_x() >= 9 && player->get_location_x() < (mine->get_map_x() - 10)))
	{
		mine_x = player->get_location_x() - 7;
		if((way == RIGHT || way == LEFT))
		{
			animate_horiz = true;
		}
	}
	else if(player->get_location_x() >= (mine->get_map_x() - 10))
	{
		mine_x = (mine->get_map_x() - 16);
	}
	
	if((player->get_location_y() > 5 && player->get_location_y() < (mine->get_map_y() - 4)))
	{
		mine_y = player->get_location_y() - 4;
		if((way == UP || way == DOWN))
		{
			animate_vert = true;
		}
	}
	else if(player->get_location_y() >= (mine->get_map_y() - 4))
	{
		mine_y = (mine->get_map_y() - 8);
	}
		
	// Blit the graphics.
	display_background_layer(player, mine, way, animate_vert, animate_horiz, mine_x, mine_y);
	display_sprite_layer(player, mine, way, animate_vert, animate_horiz, mine_x, mine_y);
	if(mine->return_recently_found_material() != NOTHING)
	{
		display_found_minerals_animated(player, mine, way, animate_vert, animate_horiz, mine_x, mine_y);
//		mine->count_recently_found();
	}

	// Display the graphics.
	display_hud(player);
	SDL_Flip(return_screen());
	
//	SDL_Delay(MINE_ANIMATION_WAIT);
}

void SDL_Objects::display_background_layer(PlayerData *player, MineData *mine, direction way, bool animate_vert, bool animate_horiz,
											int mine_x, int mine_y)
{
	int x_tile_position = 0;
	int y_tile_position = 0;
		
	// Show the area of the screen that is to be shown.
	// Below variables store position on the 48x48 grid.
	if(animate_vert == true && way == UP)
	{	
		y_tile_position = -24;
	}
	else if(animate_vert == true && way == DOWN)
	{
		y_tile_position = -24;
		mine_y = mine_y - 1;
	}
	
	if(animate_horiz == true && way == LEFT)
	{		
		x_tile_position = -24;
	}
	else if(animate_horiz == true && way == RIGHT)
	{
		x_tile_position = - 24;
		mine_x = mine_x - 1;
	}

	// Apply the background layer
	for(int y = mine_y; y < (mine_y + 11); y++)
	{
		for(int x = mine_x; x < (mine_x + 17); x++)
		{
			// Apply the appropriate graphic for the location.
			if(mine->get_explored(x,y) == false)
			{
				apply_surface(x_tile_position, y_tile_position, dirt_graphic, return_screen());	
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) != ELEVATOR 
					&& mine->get_contents(x,y) != SHAFT
					&& mine->get_contents(x,y) != GRANITE
					&& mine->get_contents(x,y) != SPRING
					&& mine->get_contents(x,y) != WATER
					&& mine->get_contents(x,y) != CAVE_IN
					&& mine->get_contents(x,y) != COAL
					&& mine->get_contents(x,y) != SILVER
					&& mine->get_contents(x,y) != GOLD
					&& mine->get_contents(x,y) != PLATINUM
					&& mine->get_contents(x,y) != DYNAMITE)
			{
				apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
			}		
			// Allow the elevator to vary with going up and down.
			// Below code is sloppy, but since the elevator is a background item
			// it is needed to have such complicated code.
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == SHAFT
					&& (player->get_location_x() != x && player->get_location_x() != y + 1)
					&& animate_vert == true)
			{
				apply_surface(x_tile_position, y_tile_position, mineshaft_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == SHAFT)
			{
				apply_surface(x_tile_position, y_tile_position, mineshaft_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == GRANITE)
			{
				apply_surface(x_tile_position, y_tile_position, granite_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == CAVE_IN)
			{
				apply_surface(x_tile_position, y_tile_position, cave_in_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == SPRING)
			{
				apply_surface(x_tile_position, y_tile_position, spring_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == WATER)
			{
				apply_surface(x_tile_position, y_tile_position, water_graphic, return_screen());
			}
			// The below else ifs come into effect if dynamite has uncovered minerals.
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == COAL)
			{
				apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
				apply_surface(x_tile_position, y_tile_position, coal_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == SILVER)
			{
				apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
				apply_surface(x_tile_position, y_tile_position, silver_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == GOLD)
			{
				apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
				apply_surface(x_tile_position, y_tile_position, gold_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == PLATINUM)
			{
				apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
				apply_surface(x_tile_position, y_tile_position, platinum_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == DYNAMITE)
			{
				apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
				apply_surface(x_tile_position, y_tile_position, dynamite_graphic, return_screen());				
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == DIAMOND)
			{
				apply_surface(x_tile_position, y_tile_position, explored_graphic, return_screen());
				apply_surface(x_tile_position, y_tile_position, diamond_graphic, return_screen());								
			}
			x_tile_position += 48;
		}
		if(animate_horiz == true){x_tile_position = -24;}
		else{x_tile_position = 0;}

		y_tile_position += 48;
	}
}

void SDL_Objects::display_sprite_layer(PlayerData *player, MineData *mine, direction way, bool animate_vert, bool animate_horiz,
										int mine_x, int mine_y)
{
	int y_tile_position = 0;
	int x_tile_position = 0;
		
	// Show the area of the screen that is to be shown.
	// Below variables store position on the 48x48 grid.
	if(animate_vert == true && way == UP)
	{
		y_tile_position = -24;	
	}
	else if(animate_vert == true && way == DOWN)
	{
		y_tile_position = -24;
		mine_y = mine_y - 1;
	}
	
	if(animate_horiz == true && way == LEFT)
	{
		x_tile_position = -24;
	}
	else if(animate_horiz == true && way == RIGHT)
	{
		x_tile_position = -24;
		mine_x = mine_x - 1;
	}
	
	// Apply the sprite layer.
	for(int y = mine_y; y < (mine_y + 11); y++)
	{
		for(int x = mine_x; x < (mine_x + 17); x++)
		{		
			// Apply the elevator on screen.
			if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == ELEVATOR
					&& way == UP
					&& (player->get_location_x() == x && player->get_location_y() == y))
			{
					apply_surface(x_tile_position, y_tile_position, mineshaft_graphic, return_screen());
					apply_surface(x_tile_position, y_tile_position + 24, elevator_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == ELEVATOR
					&& way == DOWN
					&& (player->get_location_x() == x && player->get_location_y() == y))
			{
					apply_surface(x_tile_position, y_tile_position, mineshaft_graphic, return_screen());
					apply_surface(x_tile_position, y_tile_position - 24, elevator_graphic, return_screen());				
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == ELEVATOR
					&& (player->get_location_x() != x && player->get_location_y() != y))
			{
				apply_surface(x_tile_position, y_tile_position, elevator_graphic, return_screen());
			}
			else if(mine->get_explored(x,y) == true
					&& mine->get_contents(x,y) == ELEVATOR
					&& animate_horiz == false)
			{
				apply_surface(x_tile_position, y_tile_position, elevator_graphic, return_screen());				
			}
			
			// Applies the little miner dude on the screen.
			if((x == player->get_location_x() && y == player->get_location_y()
				&& way == UP)
				&& player->get_location_y() != player->get_previous_location_y())
			{
				if(which_animation())
				{
					apply_surface(x_tile_position, y_tile_position + 24, miner_down_graphic_1, return_screen());
				}
				else 
				{
					apply_surface(x_tile_position, y_tile_position + 24, miner_down_graphic_2, return_screen());
				}
			}
			else if(x == player->get_location_x() && y == player->get_location_y() 
					&& way == DOWN
					&& player->get_location_y() != player->get_previous_location_y()
					&& player->get_location_y() != mine->get_map_y())
			{
				if(which_animation())
				{
					apply_surface(x_tile_position, y_tile_position - 24, miner_down_graphic_1, return_screen());
				}
				else 
				{
					apply_surface(x_tile_position, y_tile_position - 24, miner_down_graphic_2, return_screen());
				}
			}
			else if(x == player->get_location_x() && y == player->get_location_y() 
					&& way == LEFT
					&& player->get_location_x() != player->get_previous_location_x())
			{
				if(which_animation())
				{
					apply_surface(x_tile_position + 24, y_tile_position, miner_down_graphic_1, return_screen());
				}
				else 
				{
					apply_surface(x_tile_position + 24, y_tile_position, miner_down_graphic_2, return_screen());
				}
			}
			else if(x == player->get_location_x() && y == player->get_location_y()
					&& way == RIGHT
					&& player->get_location_x() != player->get_previous_location_x())
			{
				if(which_animation())
				{
					apply_surface(x_tile_position - 24, y_tile_position, miner_down_graphic_1, return_screen());
				}
				else 
				{
					apply_surface(x_tile_position - 24, y_tile_position, miner_down_graphic_2, return_screen());
				}
			}
			else if(x == player->get_location_x() && y == player->get_location_y())
			{
				apply_surface(x_tile_position, y_tile_position, miner_move_graphic, return_screen());
			}
									
			x_tile_position += 48;
		}
		
		if(animate_horiz == true) { x_tile_position = -24; }
		else { x_tile_position = 0; }

		y_tile_position += 48;
	}
}

void SDL_Objects::display_found_minerals_animated(PlayerData *player, MineData *mine, direction way,
											bool animate_vert, bool animate_horiz, int mine_x, int mine_y)
{
	int y_tile_position = 0;
	int x_tile_position = 0;
		
	// Show the area of the screen that is to be shown.
	// Below variables store position on the 48x48 grid.
	if(animate_vert == true && way == UP)
	{	
		y_tile_position = -24;
	}
	else if(animate_vert == true && way == DOWN)
	{
		y_tile_position = -24;
		mine_y = mine_y - 1;
	}
	
	if(animate_horiz == true && way == LEFT)
	{		
		x_tile_position = -24;
	}
	else if(animate_horiz == true && way == RIGHT)
	{
		x_tile_position = -24;
		mine_x = mine_x - 1;
	}

	for(int y = mine_y; y < (mine_y + 11); y++)
	{
		for(int x = mine_x; x < (mine_x + 17); x++)
		{	
			if(x == mine->return_recently_found_x() && y == mine->return_recently_found_y())
			{
				if(mine->return_recently_found_material() == COAL)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), coal_graphic, return_screen());
				}
				else if(mine->return_recently_found_material() == SILVER)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), silver_graphic, return_screen());
				}
				else if(mine->return_recently_found_material() == GOLD)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), gold_graphic, return_screen());
				}
				else if(mine->return_recently_found_material() == PLATINUM)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), platinum_graphic, return_screen());
				}
				else if(mine->return_recently_found_material() == DIAMOND)
				{
					apply_surface(x_tile_position,
						y_tile_position - 96 + (mine->return_recently_found_countdown() * 4), diamond_graphic, return_screen());
				}					
			}				
			x_tile_position += 48;
		}
	
		if(animate_horiz == true) { x_tile_position = -24; }
		else { x_tile_position = 0; }
			y_tile_position += 48;
	}
}

// Function to tell which animation graphic to use
bool SDL_Objects::which_animation()
{
	if(miner_animate)
	{
		miner_animate = false;
		return true;
	}
	else if(!miner_animate)
	{
		miner_animate = true;
		return false;
	}
}

// Updates the text in the HUD and clears out older information.
void SDL_Objects::update_status_text(std::string new_text)
{
	for(int x = 6; x >= 0; x--)
	{
		player_status[x + 1] = player_status[x];
	}
	
	player_status[0] = new_text;
}

// Provides the text within player_status
std::string SDL_Objects::return_status_text(int array)
{
	return player_status[array];
}

// Clear out the text in the HUD.
void SDL_Objects::clear_status_text()
{
	for(int x = 6; x >= 0; x--)
	{
		player_status[x] = "";
	}
}

// Returns the pointer to the main screen.
SDL_Surface* SDL_Objects::return_screen()
{
	return screen;
}

// Allows an instance of SDL_Surface to be applied to another surface.
void SDL_Objects::apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
	// Make a temporary rectangle to hold the offsets.
	SDL_Rect offset;
	
	// Apply the offsets to the above.
	offset.x = x;
	offset.y = y;
	
	// Blit the surface.
	SDL_BlitSurface(source, NULL, destination, &offset);
}

// Allows a line of text via SDL_ttf to be applied to a surface
void SDL_Objects::apply_text(int x, int y, std::string input_string, TTF_Font *font, SDL_Surface *destination)
{
	// Make a temporary surface to hold the text.
	SDL_Surface *temp_surface;
	
	// Color of the text
	SDL_Color text_color;
		text_color.r = 255;
		text_color.g = 255;
		text_color.b = 255;
	
	temp_surface = TTF_RenderText_Blended(font, input_string.c_str(), text_color);
	
	apply_surface(x, y, temp_surface, destination);
	
	SDL_FreeSurface(temp_surface);
}

// Allows a colored line of text to be applied to a surface
void SDL_Objects::apply_colored_text(int x, int y, int r, int g, int b, std::string input_string, TTF_Font *font, SDL_Surface *destination)
{
	// Make a temporary surface to hold the text.
	SDL_Surface *temp_surface;
	
	// Color of the text
	SDL_Color text_color;
		text_color.r = r;
		text_color.g = g;
		text_color.b = b;
	
	temp_surface = TTF_RenderText_Blended(font, input_string.c_str(), text_color);
	
	apply_surface(x, y, temp_surface, destination);
	
	SDL_FreeSurface(temp_surface);
}

// Closes out SDL
void SDL_Objects::quit_sdl()
{
	set_quitSDL();
	
	SDL_Quit();
}

void SDL_Objects::set_quitSDL()
{
	quitSDL = true;
}

bool SDL_Objects::return_quitSDL()
{
	return quitSDL;
	
}

// Quit to the main menu.
void SDL_Objects::set_quit_to_menu(bool value)
{
	quit_to_menu = value;
}

bool SDL_Objects::return_quit_to_menu()
{
	return quit_to_menu;
}

// Initialize the selection arrow object
Selection_Arrow::Selection_Arrow(int avail_horiz, int avail_vert, int amnt_x, int amnt_y)
{
	available_horiz = avail_horiz;
	available_vert = avail_vert;
	
	amount_x = amnt_x;
	amount_y = amnt_y;
	
	location_horiz = 0;
	location_vert = 0;
}

// Set the default location for the arrow
void Selection_Arrow::set_arrow_initial(int x, int y)
{
	arrow_start_x = x;
	arrow_start_y = y;
	
	arrow_x = x;
	arrow_y = y;
}

// Check to see if the player can move up. If so, then return TRUE.
bool Selection_Arrow::move_up()
{
	if(location_vert != 0)
	{
		arrow_y = arrow_y - amount_y;
		location_vert--;
		return true;
	}
	else if(location_vert == 0)
	{
		arrow_y = arrow_start_y + (amount_y * (available_vert - 1));
		location_vert = available_vert - 1;
		SDL_Delay(125);
		return false;
	}
	else
	{
		return false;
	}
}

// Check to see if the player can move down. If so, then return TRUE.
bool Selection_Arrow::move_down()
{
	if(location_vert + 1 != available_vert)
	{
		arrow_y = arrow_y + amount_y;
		location_vert++;
		return true;
	}
	else if(location_vert + 1 == available_vert)
	{
		arrow_y = arrow_start_y;
		location_vert = 0;
		SDL_Delay(125);
		return false;
	}
	else
	{
		return false;
	}
}

// Check to see if the player can move left. If so, then return TRUE.
bool Selection_Arrow::move_left()
{
	if(location_horiz != 0)
	{
		arrow_x = arrow_x - amount_x;
		location_horiz--;
		return true;
	}
	else
	{
		return false;
	}
}

// Check to see if the player can move right. If so, then return TRUE.
bool Selection_Arrow::move_right()
{
	if(location_horiz + 1 != available_horiz)
	{
		arrow_x = arrow_x + amount_x;
		location_horiz++;
		return true;
	}
	else
	{
		return false;
	}
}

// The two following functions return the current location of Selection_Arrow.
int Selection_Arrow::return_horiz()
{
	return location_horiz;
}

int Selection_Arrow::return_vert()
{
	return location_vert;
}

// The two following functions return how much the arrow is set to move with each 'event'
int Selection_Arrow::return_movement_x()
{
	return amount_x;
}

int Selection_Arrow::return_movement_y()
{
	return amount_y;
}

// The following returns the default x and y of the arrow
int Selection_Arrow::return_arrow_x()
{
	return arrow_x;
}

int Selection_Arrow::return_arrow_y()
{
	return arrow_y;
}
