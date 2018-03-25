/*
 classes.h
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
 Information for two classes pertaining to gameplay information:
 PlayerData & MineData
 
 Both keep track of information about the player's status and
 what has happened within the mine.
 */


#ifndef CLASSES
#define CLASSES

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"

class SDL_Objects;
class MineData;

// Class to hold all data pertaining to the player
class PlayerData
{
	private:
		// Storage for the player's general stats
		int money;
		int health;
		
		int coal;
		int silver;
		int gold;
		int platinum;
		
		int coal_value;
		int silver_value;
		int gold_value;
		int platinum_value;
		
		// Allows to keep track of how many turns have been taken.
		// Used to randomize mineral values at the bank and when
		// insurance was last purchased.
		int turn_number;
		int previous_turn_number;	// Keeps track of last time the bank
									// minerals were random valued.
		int insurance_turn_number;
		
		// Storage for the player's possessions
		bool has_axe;
		bool has_bucket;
		bool has_dynamite;
		bool has_flashlight;
		bool has_hardhat;
		bool has_shovel;
		bool has_diamond;
		bool has_insurance;
		
		// Variables concerned with dynamite usage.
		bool dynamite_primed;
		int dynamite_timer;
		
		// Storage for player's location in the mine
		int location_x;
		int location_y;
		int previous_location_x;
		int previous_location_y;
		
		// Store where the player has laid down dynamite
		int dynamite_x;
		int dynamite_y;
		
	public:
		PlayerData();
		
		// Change the player's general stats
		void change_money(int value);
		void change_health(int value);
		void change_coal(int value);
		void change_silver(int value);
		void change_gold(int value);
		void change_platinum(int value);
		void randomize_coal_value();
		void randomize_silver_value();
		void randomize_gold_value();
		void randomize_platinum_value();
		
		// Retrieve the player's general stats
		int get_money();
		int get_health();
		int get_coal();
		int get_silver();
		int get_gold();
		int get_platinum();
		int get_coal_value();
		int get_silver_value();
		int get_gold_value();
		int get_platinum_value();
		
		// Change the player's possessions
		void change_has_axe(bool value);
		void change_has_bucket(bool value);
		void change_has_dynamite(bool value);
		void change_has_flashlight(bool value);
		void change_has_hardhat(bool value);
		void change_has_shovel(bool value);
		void change_has_diamond(bool value);
		void change_has_insurance(bool value);
		
		// Automatically effect stats according to inventory
		void dig_function();
		
		// Modify and access the player's number of turns.
		void increment_turn_number();
		void set_previous_turn_number();
		void set_previous_turn_number(int number);
		void set_insurance_turn_number();
		void set_insurance_turn_number(int number);
		void set_turn_number(int number);
		int get_previous_turn_number();
		int get_insurance_turn_number();
		int get_turn_number();
		
		// Retrieve the player's possession status
		bool get_has_axe();
		bool get_has_bucket();
		bool get_has_dynamite();
		bool get_has_flashlight();
		bool get_has_hardhat();
		bool get_has_shovel();
		bool get_has_diamond();
		bool get_has_insurance();
		
		// Allows the player's health and money status to be checked each turn.
		bool check_health(SDL_Objects *sdl);
 		
		// Primes the player's dynamite for explosion.
		void dynamite_prime(int x, int y, MineData *mine);
		
		// Performs the countdown for the dynamite.
		// Returns true if the dynamite explodes.
		bool dynamite_countdown(MineData *mine);
		
		// Check to see whether the player is inside the blast radius of the dynamite.
		bool dynamite_radius();
		
		// Executed once the player's priming time is up.
		void dynamite_explode(int x, int y, MineData *mine);
		
		// Change the player's location in the mine.
		void change_location(int x, int y, MineData *mine, SDL_Objects *sdl);
		
		// Check to see if a player can move to a certain area.
		bool valid_location(int x, int y, MineData *mine);
		
		// Sets the location of the dynamite.
		void set_dynamite_location(int x, int y, MineData *mine);
		
		// Retrieve the x and y coords of the player.
		int get_location_x();
		int get_location_y();
		int get_previous_location_x();
		int get_previous_location_y();

		// Get the location of the dynamite.
		int get_dynamite_location_x();
		int get_dynamite_location_y();
};

// Enumeration to keep track of what is located where in the mine.
enum materials{
	DIRT,		// ENUM 0 (used to show unexplored areas)
	GRANITE,	// ENUM 1 (ued to show granite)
	CAVE_IN,	// ENUM 2 (used to store cave in locations)
	SPRING,		// ENUM 3 (used to store where springs are)
	COAL,		// ENUM 4 (used to store coal locations)
	SILVER,		// ENUM 5 (used to store silver locations)
	GOLD,		// ENUM 6 (used to store where gold is)
	PLATINUM,	// ENUM 7 (used to store where platinum is)
	EXPLORED,	// ENUM 8 (used to store where a player has explored)
	SHAFT,		// ENUM 9 (used to store the location of the elevator shaft)
	ELEVATOR,	// ENUM 10 (used to store the location of the elevator)
	WATER,		// ENUM 11 (used to store water from a spring)
	DYNAMITE,	// ENUM 12 (used to store where dynamite has been placed)
	DIAMOND,	// ENUM 13 (the diamond that mimi's been looking for!)
	NOTHING		// ENUM 14 (used for when the user hasn't found anything there recently)
};


// Class to hold all data pertaining to the mining field
class MineData
{
	private:
		// Refers to above enumeration.		
		materials mine_contents[192][192];
		
		// Refers to whether the player has explored an area or not.
		bool explored[192][192];
		
		// Stores where the diamond is located.
		int diamond_x;
		int diamond_y;
		
		// Map size in tiles
		int map_x;
		int map_y;
		
		// Information for what has been recently found.
		materials recently_found_material;
		int recently_found_x;
		int recently_found_y;
		int recently_found_countdown;
		
	public:
		// Class initializer.
		MineData();	
		~MineData();
		
		// Randomize the mine.
		void randomize_mine();
		
		// Allows to check what is stored in a certain area.
		materials get_contents(int x, int y);

		// Allows to check whether an area has been explored or not.
		bool get_explored(int x, int y);
		
		// Allows materials to be stored into mine_contents.
		// An area can be marked as being explored by sending EXPLORED
		// to contents.
		void set_contents(int x, int y, materials contents);
		
		// Allows the explored status to be changed.
		void set_explored(int x, int y, bool status);
		
		// Simulates the mine caving in.
		void cave_in(int x, int y);
		
		// Simulates the mine being flooded by a spring.
		void water_flow(int x, int y);
		
		// Gets where the diamond is located.
		int get_diamond_x();
		int get_diamond_y();
		
		// Returns the dimensions of the mine
		int get_map_x();
		int get_map_y();
		
		// Allow a recently found item to be set. (ANIMATION OF MINERALS)
		void add_recently_found(int x, int y, materials contents);
		
		// Increment the counter for recently found. (ANIMATION OF MINERALS)
		void count_recently_found();
		
		// Returns for the values within the recently founds.
		// Used for the animation of found minerals.
		materials return_recently_found_material();
		int return_recently_found_x();
		int return_recently_found_y();
		int return_recently_found_countdown();
		
		// Used to set the location of the diamond for when the game is loaded.
		void set_diamond_location(int x, int y);
};

#endif
