/*
 classes.cpp
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

#include <ctime>		// Allows generation of psuedorandom numbers.
#include <cstdlib>		// Allows for rand()
#include <iostream>		// For testing purposes... cout.

#include "sdl_functions.h"
#include "classes.h"

// PlayerData constructor
PlayerData::PlayerData()
{
	// Seed the random number generator.
	srand( time(NULL) );
	
	// Set money and health to default starting values.
	money = 1500;
	health = 100;
	
	// Set mineral values to zero. You start with nothing, sucka.
	coal = 0;
	silver = 0;
	gold = 0;
	platinum = 0;
	
	coal_value = 15;
	silver_value = 50;
	gold_value = 100;
	platinum_value = 250;

	turn_number = 0;
	previous_turn_number = 0;
	insurance_turn_number = -100;
	
	has_axe = false;
	has_bucket = false;
	has_dynamite = false;
	has_flashlight = false;
	has_hardhat = false;
	has_shovel = false;
	has_diamond = false;
	has_insurance = false;

	location_x = 0;
	location_y = 0;
	previous_location_x = 0;
	previous_location_y = 0;
		
	dynamite_x = 0;
	dynamite_y = 0;
	
	dynamite_primed = false;
	dynamite_timer = 0;
}

void PlayerData::change_money(int value)
{
	money += value;
}

void PlayerData::change_health(int value)
{
	health += value;
}

void PlayerData::change_coal(int value)
{
	coal += value;
}

void PlayerData::change_silver(int value)
{
	silver += value;
}

void PlayerData::change_gold(int value)
{
	gold += value;
}

void PlayerData::change_platinum(int value)
{
	platinum += value;
}

void PlayerData::randomize_coal_value()
{
	coal_value = rand() % 25;

	while(coal_value < 5 || coal_value > 25)
	{
		coal_value = rand() % 25;
	}
}

void PlayerData::randomize_silver_value()
{
	silver_value = rand() % 60;

	while(silver_value < 40 || silver_value > 60)
	{
		silver_value = rand() % 60;
	}
}

void PlayerData::randomize_gold_value()
{
	gold_value = rand() % 125;

	while(gold_value < 80 || gold_value > 125)
	{
		gold_value = rand() % 125;
	}
}

void PlayerData::randomize_platinum_value()
{
	platinum_value = rand() % 275;

	while(platinum_value < 225 || platinum_value > 275)
	{
		platinum_value = rand() % 275;
	}
}
		
int PlayerData::get_money()
{
	return money;
}

int PlayerData::get_health()
{
	return health;
}

int PlayerData::get_coal()
{
	return coal;
}

int PlayerData::get_silver()
{
	return silver;
}

int PlayerData::get_gold()
{
	return gold;
}

int PlayerData::get_platinum()
{
	return platinum;
}

int PlayerData::get_coal_value()
{
	return coal_value;
}

int PlayerData::get_silver_value()
{
	return silver_value;
}

int PlayerData::get_gold_value()
{
	return gold_value;
}

int PlayerData::get_platinum_value()
{
	return platinum_value;
}
	
void PlayerData::change_has_axe(bool value)
{
	has_axe = value;
}

void PlayerData::change_has_bucket(bool value)
{
	has_bucket = value;
}

void PlayerData::change_has_dynamite(bool value)
{
	has_dynamite = value;
}

void PlayerData::change_has_flashlight(bool value)
{
	has_flashlight = value;
}

void PlayerData::change_has_hardhat(bool value)
{
	has_hardhat = value;
}

void PlayerData::change_has_shovel(bool value)
{
	has_shovel = value;
}

void PlayerData::change_has_diamond(bool value)
{
	has_diamond = value;
}

void PlayerData::change_has_insurance(bool value)
{
	has_insurance = value;
}

void PlayerData::dig_function()
{
	// Check to see if the player has the shovel. Adjust money accordingly.
	if(get_has_shovel() == false)
	{
		change_money(-20);
	}
	else
	{
		change_money(-15);
	}
}

void PlayerData::increment_turn_number()
{
	turn_number++;
	
	if((get_insurance_turn_number() + 50) < get_turn_number())
	{
		change_has_insurance(false);
	}
}

void PlayerData::set_previous_turn_number()
{
	previous_turn_number = turn_number;
}

void PlayerData::set_previous_turn_number(int number)
{
	previous_turn_number = number;
}

void PlayerData::set_insurance_turn_number()
{
	insurance_turn_number = turn_number;
}

void PlayerData::set_insurance_turn_number(int number)
{
	insurance_turn_number = number;
}

void PlayerData::set_turn_number(int number)
{
	turn_number = number;
}

int PlayerData::get_previous_turn_number()
{
	return previous_turn_number;
}

int PlayerData::get_insurance_turn_number()
{
	return insurance_turn_number;
}

int PlayerData::get_turn_number()
{
	return turn_number;
}
		
bool PlayerData::get_has_axe()
{
	return has_axe;
}

bool PlayerData::get_has_bucket()
{
	return has_bucket;
}

bool PlayerData::get_has_dynamite()
{
	return has_dynamite;
}

bool PlayerData::get_has_flashlight()
{
	return has_flashlight;
}

bool PlayerData::get_has_hardhat()
{
	return has_hardhat;
}

bool PlayerData::get_has_shovel()
{
	return has_shovel;
}

bool PlayerData::get_has_diamond()
{
	return has_diamond;
}

bool PlayerData::get_has_insurance()
{
	return has_insurance;
}

// Allows the player's health to be checked after each turn.
// Return false if health is below 0.
bool PlayerData::check_health(SDL_Objects *sdl)
{
	if(get_health() <= 0)
		{
			// If player has no insurance, they die.
			if(get_has_insurance() == false)
			{
				// Kick the player out of the mine.
				return false;
			}
			else if((get_insurance_turn_number() + 50) > get_turn_number()
					&& get_has_insurance() == true)
			{
				// Give the player 25 health to help them out.
				health = 35;
				
				// Remove the player's insurance.
				change_has_insurance(false);

				// Update onscreen information with the player's status.
				sdl->update_status_text("Thank goodness for insurance!");
				
				return true;
			}
		}
	return true;
}

// Primes the player's dynamite for explosion.
void PlayerData::dynamite_prime(int x, int y, MineData *mine)
{
	mine->set_contents(x ,y, DYNAMITE);	// Change the tile to dynamite.
	set_dynamite_location(x ,y, mine);	// Let the program know where the dynamite is.
	dynamite_primed = true;				// convert this to a function.
	change_has_dynamite(false);
	dynamite_timer = 0;					// Set the timer for the dynamite.
}

bool PlayerData::dynamite_countdown(MineData *mine)
{
	if(dynamite_primed == true && dynamite_timer < 1)
	{
		dynamite_timer++;
		return false;
	}
	else if(dynamite_primed == true && dynamite_timer >= 1)
	{	
		// Check to see if the player is in the blast radius.
		if(dynamite_radius())
		{
			// Blow up the dynamite.
			dynamite_explode(get_dynamite_location_x(), get_dynamite_location_y(), mine);
			set_dynamite_location(0, 0, mine);			// Clear the dynamite variables.
			dynamite_primed = false;			
			return true;
		}
		else
		{
			// Blow up the dynamite.
			dynamite_explode(get_dynamite_location_x(), get_dynamite_location_y(), mine);
			set_dynamite_location(0, 0, mine);			// Clear the dynamite variables.
			dynamite_primed = false;
			return false;
		}
	}
	
	return false;
}

// Check to see if the player is within the blast ardius of the dynamite.
bool PlayerData::dynamite_radius()
{
	if(((get_location_x() < (get_dynamite_location_x() + 2))
		&& (get_location_x() > (get_dynamite_location_x() - 2)))
		&& ((get_location_y() < (get_dynamite_location_y() + 2))
		&& (get_location_y() > (get_dynamite_location_y() - 2))))
	{
		return true;
	}
	else
	{
		return false;
	}
}
		
// Executed once the player's dynamite is past its priming time.
void PlayerData::dynamite_explode(int x, int y, MineData *mine)
{
	for(int x_start = x - 2; x_start <= x + 2; x_start++)
	{
		for(int y_start = y - 2; y_start <= y + 2; y_start++)
		{
			if(x_start >= 0 && y_start >= 0)
			{
				// Set all tiles within blast to explored.
				mine->set_explored(x_start, y_start, true);
		
				// Clear the tile if the material is a hazard.
				if((mine->get_contents(x_start, y_start) == GRANITE)
					|| (mine->get_contents(x_start, y_start) == SPRING)
					|| (mine->get_contents(x_start, y_start) == WATER)
					|| (mine->get_contents(x_start, y_start) == CAVE_IN)
					|| (mine->get_contents(x_start, y_start) == DYNAMITE)) // Clear the dynamite's tile
				{
					mine->set_contents(x_start, y_start, EXPLORED);
				}
			}
		}
	}
}
		
void PlayerData::change_location(int x, int y, MineData *mine, SDL_Objects *sdl)
{	
	// Below if statments ensure that the requested move is valid.
	if((x >= 0 && x < mine->get_map_x()) && (y >= 0 && y < mine->get_map_y()))
	{
		previous_location_x = location_x;
		previous_location_y = location_y;
		
		// Following is executed if the move is valid.
		if(mine->get_explored(x,y) == false
			&& mine->get_contents(x,y) != GRANITE
			&& mine->get_contents(x,y) != SPRING
			&& mine->get_contents(x,y) != WATER
			&& mine->get_contents(x,y) != CAVE_IN
			&& mine->get_contents(x,y) != COAL
			&& mine->get_contents(x,y) != SILVER
			&& mine->get_contents(x,y) != GOLD
			&& mine->get_contents(x,y) != PLATINUM
			&& mine->get_contents(x,y) != WATER
			&& mine->get_contents(x,y) != DIAMOND)
		{
			// Set this area to being explored.
			mine->set_explored(x,y, true);
			
			// Dig the area, subtract money.
			dig_function();
			
			// Increment the number of moves the player has performed.
			increment_turn_number();
			
			// Update the player's location.
			location_x = x;
			location_y = y;
		}
		else if(mine->get_contents(x,y) == GRANITE)
		{
			// Dig the area, subtract money.
			if(mine->get_explored(x,y) == false)
			{
				dig_function();
				
				// Make the granite known
				mine->set_explored(x,y, true);
				
				// Increment the number of moves the player has performed.
				increment_turn_number();
			}
			else if(get_has_axe())
			{
				// Charge the player a slightly higher rate for digging.
				change_money(-30);
				
				// Change the granite area to an explored area.
				mine->set_contents(x,y, EXPLORED);
				
				// Increment the number of moves the player has performed.
				increment_turn_number();
				
				// Update the HUD
				sdl->update_status_text("You chip away at the granite!");
			}
			else
			{
				// Update the HUD
				sdl->update_status_text("You can't dig through granite!");
			}
		}
		else if(mine->get_contents(x,y) == SPRING)
		{
			// Dig the area, subtract money.
			dig_function();
			
			// Make the spring known
			mine->set_explored(x,y, true);
			
			// Subtract from the player's health.
			change_health(-10);
			
			// Increment the number of moves the player has performed.
			increment_turn_number();
			
			// Have the spring's water flow about.
			mine->water_flow(x,y);
			
			// Update the HUD
			sdl->update_status_text("Oh no, a spring!");
		}
		else if(mine->get_contents(x,y) == CAVE_IN)
		{
			// Dig the area, subtract money.
			dig_function();
			
			// Make the cave-in known
			mine->set_explored(x,y, true);
			
			// Subtract from the players health, check to see if helmet is worn.
			if(get_has_hardhat() == true)
			{
				change_health(-5);
			}
			else
			{
				change_health(-10);
			}
			
			// Increment the number of moves the player has performed.
			increment_turn_number();
			
			// Change the area around the cave-in
			mine->cave_in(x, y);
			
			// Update the HUD
			sdl->update_status_text("Ow, a cave-in!");
		}
		else if(mine->get_contents(x,y) == COAL)
		{
			if(!mine->get_explored(x,y))
			{
				// Dig the area, subtract money.
				dig_function();
			}
			
			// Randomly adjust the amount of materials found.
			change_coal((rand() % 4) + 1);
			
			// Increment the number of moves the player has performed.
			increment_turn_number();
						
			// Make the area known
			mine->set_explored(x,y, true);
			mine->set_contents(x,y, EXPLORED);

			// Update the player's location.
			location_x = x;
			location_y = y;
			
			// Begin the animation of finding the item.
			mine->add_recently_found(location_x, location_y, COAL);
			
			// Update the HUD
			sdl->update_status_text("You found some coal!");			
		}
		else if(mine->get_contents(x,y) == SILVER)
		{
			if(!mine->get_explored(x,y))
			{
				// Dig the area, subtract money.
				dig_function();
			}
			
			// Randomly adjust the amount of minerals found.
			change_silver((rand() % 3) + 1);
			
			// Increment the number of moves the player has performed.
			increment_turn_number();			
			
			// Make the area known
			mine->set_explored(x,y, true);
			mine->set_contents(x,y, EXPLORED);
			
			// Update the player's location.
			location_x = x;
			location_y = y;
			
			// Begin the animation of finding the item.
			mine->add_recently_found(location_x, location_y, SILVER);

			// Update the HUD
			sdl->update_status_text("You found some silver!");
		}
		else if(mine->get_contents(x,y) == GOLD)
		{
			if(!mine->get_explored(x,y))
			{
				// Dig the area, subtract money.
				dig_function();
			}
			
			// Randomly adjust the amount of minerals found.	
			change_gold((rand() % 3) + 1);
			
			// Increment the number of moves the player has performed.
			increment_turn_number();		
			
			// Make the area known
			mine->set_explored(x,y, true);
			mine->set_contents(x,y, EXPLORED);
			
			// Update the player's location.
			location_x = x;
			location_y = y;

			// Begin the animation of finding the item.
			mine->add_recently_found(location_x, location_y, GOLD);
			
			// Update the HUD
			sdl->update_status_text("You found some gold!");
		}
		else if(mine->get_contents(x,y) == PLATINUM)
		{
			if(!mine->get_explored(x,y))
			{
				// Dig the area, subtract money.
				dig_function();
			}
			
			// Randomly adjust the amount of minerals found.
			change_platinum((rand() % 2) + 1);
			
			// Make the area known
			mine->set_explored(x,y, true);
			mine->set_contents(x,y, EXPLORED);
			
			// Increment the number of moves the player has performed.
			increment_turn_number();		
			
			// Update the player's location.
			location_x = x;
			location_y = y;

			// Begin the animation of finding the item.
			mine->add_recently_found(location_x, location_y, PLATINUM);
			
			// Update the HUD
			sdl->update_status_text("You found some platinum!");			
		}
		// Deal with the character trying to enter a stream of water.
		else if(mine->get_contents(x,y) == WATER)
		{
			// Check to see whether the player has the bucket.
			if(get_has_bucket())
			{
				// Allow the player to deal with the stream, at a cost.
				
				// Increment the number of moves the player has performed.
				increment_turn_number();
				
				// Charge the character 40 dollars for clearing the stream.
				change_money(-40);

				// Change the tile to plain ol' explored
				mine->set_contents(x,y, EXPLORED);
				
				// Update the HUD
				sdl->update_status_text("You use your bucket!");
			}
			// Do not allow the player to enter the stream.
			else
			{
				// Increment the number of moves the player has performed.
				increment_turn_number();
				
				// Lose 5 health for trying to enter the stream.
				change_health(-5);
				
				// Update the HUD
				sdl->update_status_text("You start to drown!");
			}
		}
		// Deal with the player finding the diamond in the mine.
		else if(mine->get_contents(x,y) == DIAMOND)
		{
			if(!mine->get_explored(x,y))
			{
				// Dig the area, subtract money.
				dig_function();
			}
			
			// Increment the number of moves the player has performed.
			increment_turn_number();
			
			// Change the tile to plain ol' explored
			mine->set_contents(x,y, EXPLORED);
			mine->set_explored(x,y, true);
			
			// Put the player in possession of the diamond.
			change_has_diamond(true);
			
			// Update the player's location.
			location_x = x;
			location_y = y;

			// Begin the animation of finding the item.
			mine->add_recently_found(location_x, location_y, DIAMOND);
				
			// Update the HUD
			sdl->update_status_text("You found the diamond!");
		}	
		// Allow the character to move to previously searched location, except for mineshaft.
		else if(mine->get_explored(x,y) == true
				&& mine->get_contents(x,y) != SHAFT)
		{
			location_x = x;
			location_y = y;
		}
		// Moves the elevator up/down if the player is in it. 
		else if(mine->get_explored(x,y) == true
				&& mine->get_contents(location_x, location_y) == ELEVATOR)
		{
			// Move the elevator up/down.
			mine->set_contents(x,y, ELEVATOR);
			mine->set_contents(location_x, location_y, SHAFT);
			
			// Charge the player for the elevator movement if elevator is going down.
			if(location_y < y)
			{
				change_money(-1);
			}
			
			location_x = x;
			location_y = y;
		}
	}
	else if((x == 0) && (y == -1))
	{
		location_x = x;
		location_y = y;
	}
}

void PlayerData::set_dynamite_location(int x, int y, MineData *mine)
{
	if((x >= 0 && x <= mine->get_map_x()) && (y >= 0 && y <= mine->get_map_y()))
	{
		dynamite_x = x;
		dynamite_y = y;
	}
}
		
int PlayerData::get_location_x()
{
	return location_x;
}

int PlayerData::get_location_y()
{
	return location_y;
}

int PlayerData::get_previous_location_x()
{
	return previous_location_x;
}

int PlayerData::get_previous_location_y()
{
	return previous_location_y;
}

int PlayerData::get_dynamite_location_x()
{
	return dynamite_x;
}

int PlayerData::get_dynamite_location_y()
{
	return dynamite_y;
}

// MineData constructor
MineData::MineData()
{
	// Set the diamond location to zero.
	diamond_x = 0;
	diamond_y = 0;
	
	// State the dimensions of the map.
	map_x = 191;
	map_y = 191;
	
	// Clear out the recently found area.
	recently_found_material = NOTHING;
	recently_found_x = NULL;
	recently_found_y = NULL;
	recently_found_countdown = -1;
	
	// Randomize the mine.
	randomize_mine();
}

// MineData deconstructor
MineData::~MineData()
{
	
}

void MineData::randomize_mine()
{
	int tempValue = 0;
	int testedValue = 0;

	srand( time(NULL) );		// Seed the random number generator.

	// Makes the mineshaft on the far left of the matrix.
	for(int y = 0; y <= map_y; y++)
	{
		set_contents(0, y, SHAFT);
		set_explored(0, y, true);
	}

	// Randomizes the rest of the matrix. 
	for(int x = 1; x <= (map_x - 1); x++)
	{
		for(int y = 0; y <= (map_y - 1); y++)
		{
			tempValue = rand() % 7;
			
			// Sets the mine to being unexplored.
			set_explored(x, y, false);
			
			if(tempValue == 0)
			{
				testedValue = rand() % 1;

				if(testedValue == 0)
				{
					set_contents(x, y, COAL);
				}
				else
				{
					set_contents(x, y, DIRT);
				}
			}
			else if(tempValue == 1)
			{
				testedValue = rand() % 3;

				if(testedValue == 0)
				{
					set_contents(x, y, SILVER);
				}
				else
				{
					set_contents(x, y, DIRT);
				}

			}
			else if(tempValue == 2)
			{
				testedValue = rand() % 5;

				if(testedValue == 0)
				{
					set_contents(x, y, GOLD);
				}
				else
				{
					set_contents(x, y, DIRT);
				}
			}
			else if(tempValue == 3)
			{
				testedValue = rand() % 7;

				if(testedValue == 0)
				{
					set_contents(x, y, PLATINUM);
				}
				else
				{
					set_contents(x, y, DIRT);
				}
			}
			else if(tempValue == 4)
			{
				testedValue = rand() % 5;

				if(testedValue == 0)
				{
					set_contents(x, y, CAVE_IN);
				}
				else
				{
					set_contents(x, y, DIRT);
				}
			}
			else if(tempValue == 5)
			{
				testedValue = rand() % 5;

				if(testedValue == 0)
				{
					set_contents(x, y, SPRING);
				}
				else
				{
					set_contents(x, y, DIRT);
				}
			}
			else if(tempValue == 6)
			{
				testedValue = rand() % 2;

				if(testedValue == 0)
				{
					set_contents(x, y, GRANITE);
				}
				else
				{
					set_contents(x, y, DIRT);
				}
			}
		}
	}
	
	// Randomly place the diamond in one tile of the mine.
	diamond_x = rand() % map_x + 1;
	diamond_y = rand() % map_y + 1;
	set_contents(diamond_x, diamond_y, DIAMOND);
	
	// Place the elevator at the 0, 0 area of the matrix.
	set_contents(0, 0, ELEVATOR);
}

// Returns what is at a specified area in the mine.
materials MineData::get_contents(int x, int y)
{
	return mine_contents[x][y];
}

// Returns whether an area has been explored or not.
bool MineData::get_explored(int x, int y)
{
	return explored[x][y];
}

// Allows materials to be stored into mine_contents
void MineData::set_contents(int x, int y, materials contents)
{
	mine_contents[x][y] = contents;
}

// Allows the status of an explored area to be changed.
void MineData::set_explored(int x, int y, bool status)
{
	explored[x][y] = status;
}

// Simulates the mine caving in.
void MineData::cave_in(int x, int y)
{
	int tempValue = 0;
	int testedValue = 0;
	
	srand( time(NULL) );		// Seed the random number generator.
	
	// Randomizes the rest of the matrix. 
	for(int x_start = x - 1; x_start <= x + 1; x_start++)
	{
		for(int y_start = y - 1; y_start <= y + 1; y_start++)
		{
			if((x != x_start || y != y_start) 
				&& get_contents(x_start, y_start) != ELEVATOR 
				&& get_contents(x_start, y_start) != SHAFT
				&& get_contents(x_start, y_start) != DIAMOND)
			{
				tempValue = rand() % 7;
			
				// Sets the mine to being unexplored.
				set_explored(x_start, y_start, false);
			
				if(tempValue == 0)
				{
					testedValue = rand() % 1;

					if(testedValue == 0)
					{
						set_contents(x_start, y_start, COAL);
					}
					else
					{
						set_contents(x_start, y_start, DIRT);
					}
				}
				else if(tempValue == 1)
				{
					testedValue = rand() % 3;

					if(testedValue == 0)
					{
						set_contents(x_start, y_start, SILVER);
					}
					else
					{
						set_contents(x_start, y_start, DIRT);
					}

				}
				else if(tempValue == 2)
				{
					testedValue = rand() % 5;

					if(testedValue == 0)
					{
						set_contents(x_start, y_start, GOLD);
					}
					else
					{
						set_contents(x_start, y_start, DIRT);
					}
				}
				else if(tempValue == 3)
				{
					testedValue = rand() % 7;

					if(testedValue == 0)
					{
						set_contents(x_start, y_start, PLATINUM);
					}
					else
					{
						set_contents(x_start, y_start, DIRT);
					}
				}
				else if(tempValue == 4)
				{
					testedValue = rand() % 5;

					if(testedValue == 0 && y_start != 1)
					{
						set_contents(x_start, y_start, CAVE_IN);
					}
					else
					{
						set_contents(x_start, y_start, DIRT);
					}
				}
				else if(tempValue == 5)
				{
					testedValue = rand() % 5;

					if(testedValue == 0 && y_start > 2)
					{
						set_contents(x_start, y_start, SPRING);
					}
					else
					{
						set_contents(x_start, y_start, DIRT);
					}
				}
				else if(tempValue == 6)
				{
					testedValue = rand() % 2;

					if(testedValue == 0)
					{
						set_contents(x_start, y_start, GRANITE);
					}
					else
					{
						set_contents(x_start, y_start, DIRT);
					}
				}
			}
		}
	}
}

// Simulates the mine being flooded by a spring.
void MineData::water_flow(int x, int y)
{
	for(int x_start = x - 2; x_start <= x + 2; x_start++)
	{
		for(int y_start = y - 1; y_start <= y + 1; y_start++)
		{
			if((get_explored(x_start, y_start) == true)
				&& (get_contents(x_start, y_start) != GRANITE)
				&& (get_contents(x_start, y_start) != SPRING)
				&& (get_contents(x_start, y_start) != ELEVATOR)
				&& (get_contents(x_start, y_start) != SHAFT)
				&& (get_contents(x_start, y_start) != DIAMOND))
			{
				// Stop the water from flowing the elevator over.
				if(x_start > 2)
				{
					set_contents(x_start, y_start, WATER);
				}
			}
		}
	}
}

// Returns the location of the diamond
int MineData::get_diamond_x()
{
	return diamond_x;
}

int MineData::get_diamond_y()
{
	return diamond_y;
}

// Return the dimensions of the map
int MineData::get_map_x()
{
	return map_x;
}

int MineData::get_map_y()
{
	return map_y;
}

// Allow a recently found item to be set.
void MineData::add_recently_found(int x, int y, materials contents)
{
	recently_found_material = contents;
	recently_found_x = x;
	recently_found_y = y;
	recently_found_countdown = 24;	
}
		
// Increment the counter for recently found.
void MineData::count_recently_found()
{
	if(recently_found_countdown > 0)
	{
		recently_found_countdown--;
	}
	else
	{
		recently_found_material = NOTHING;
		recently_found_x = NULL;
		recently_found_y = NULL;
		recently_found_countdown = -1;
	}
}


// Returns for all of the values that are associated with the recently founds.
materials MineData::return_recently_found_material()
{
	return recently_found_material;
}

int MineData::return_recently_found_x()
{
	return recently_found_x;
}

int MineData::return_recently_found_y()
{
	return recently_found_y;
}

int MineData::return_recently_found_countdown()
{
	return recently_found_countdown;
}

// Used to set the location of the diamond for when the game is loaded.
void MineData::set_diamond_location(int x, int y)
{
	diamond_x = x;
	diamond_y = y;
}
