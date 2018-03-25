/*
 save_load.cpp
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
 Save and load functions.
  
 Allows the program to access functions that save and load information
 about the player's status and the mine's status.
  
 All work in these files are done 'under the hood.' 
*/

#include <iostream>
#include <fstream>

#include "classes.h"

void save_game(MineData *mine, PlayerData *player)
{
	// Save the player's information to the player file.
	std::ofstream player_out("player_save", std::ios::binary);
	
		player_out << player->get_health() << std::endl;
		player_out << player->get_money() << std::endl;
	
		player_out << player->get_coal() << std::endl;
		player_out << player->get_silver() << std::endl;
		player_out << player->get_gold() << std::endl;
		player_out << player->get_platinum() << std::endl;
	
		player_out << player->get_turn_number() << std::endl;
		player_out << player->get_insurance_turn_number() << std::endl;
		player_out << player->get_previous_turn_number() << std::endl;
	
		player_out << player->get_has_axe() << std::endl;
		player_out << player->get_has_bucket() << std::endl;
		player_out << player->get_has_diamond() << std::endl;
		player_out << player->get_has_dynamite() << std::endl;
		player_out << player->get_has_flashlight() << std::endl;
		player_out << player->get_has_hardhat() << std::endl;
		player_out << player->get_has_insurance() << std::endl;
		player_out << player->get_has_shovel() << std::endl;
	
	player_out.close();
	
	std::ofstream mine_out("mine_save", std::ios::binary);
	
		// Dump the mine contents to the file.
		for(int x = 0; x <= (mine->get_map_x() - 1); x++)
		{
			for(int y = 0; y <= (mine->get_map_y() - 1); y++)
			{
				mine_out << mine->get_contents(x, y) << std::endl;
			}
		}
		
		for(int x = 0; x <= (mine->get_map_x() - 1); x++)
		{
			for(int y = 0; y <= (mine->get_map_y() - 1); y++)
			{
				mine_out << mine->get_explored(x, y) << std::endl;
			}
		}
		
		// Get the location of the diamond (for the hint screen)
		mine_out << mine->get_diamond_x() << std::endl;
		mine_out << mine->get_diamond_y() << std::endl;
	
	mine_out.close();
}

void load_game(MineData *mine, PlayerData *player)
{
	int temp_int = 0;
	bool temp_bool = false;
	int temp_x = 0;
	int temp_y = 0;
	
	// Load the player's information from the player file
	std::ifstream player_in("player_save", std::ios::binary);
	
		player_in >> temp_int;
		player->change_health(temp_int - player->get_health());
	
		player_in >> temp_int;
		player->change_money(temp_int - player->get_money());
	
		player_in >> temp_int;
		player->change_coal(temp_int - player->get_coal());
	
		player_in >> temp_int;
		player->change_silver(temp_int - player->get_silver());
	
		player_in >> temp_int;
		player->change_gold(temp_int - player->get_gold());
	
		player_in >> temp_int;
		player->change_platinum(temp_int - player->get_platinum());
	
		player_in >> temp_int;
		player->set_turn_number(temp_int);
	
		player_in >> temp_int;
		player->set_insurance_turn_number(temp_int);
	
		player_in >> temp_int;
		player->set_previous_turn_number(temp_int);
	
		player_in >> temp_bool;
		player->change_has_axe(temp_bool);
	
		player_in >> temp_bool;
		player->change_has_bucket(temp_bool);
	
		player_in >> temp_bool;
		player->change_has_diamond(temp_bool);
	
		player_in >> temp_bool;
		player->change_has_dynamite(temp_bool);
	
		player_in >> temp_bool;
		player->change_has_flashlight(temp_bool);
	
		player_in >> temp_bool;
		player->change_has_hardhat(temp_bool);
	
		player_in >> temp_bool;
		player->change_has_insurance(temp_bool);
	
		player_in >> temp_bool;
		player->change_has_shovel(temp_bool);
	
		// Randomize the mineral values.
		player->randomize_coal_value();
		player->randomize_silver_value();
		player->randomize_gold_value();
		player->randomize_platinum_value();
	
	player_in.close();
	
	std::ifstream mine_in("mine_save", std::ios::binary);
	
		// Load the mine information into the game.
		for(int x = 0; x <= (mine->get_map_x() - 1); x++)
		{
			for(int y = 0; y <= (mine->get_map_y() - 1); y++)
			{
				mine_in >> temp_int;
				mine->set_contents(x, y, (materials)temp_int);
			}
		}
		
		for(int x = 0; x <= (mine->get_map_x() - 1); x++)
		{
			for(int y = 0; y <= (mine->get_map_y() - 1); y++)
			{
				mine_in >> temp_bool;
				mine->set_explored(x, y, temp_bool);
			}
		}
	
		// Set the location of the diamond (for the hint screen)
		mine_in >> temp_x >> temp_y;
		mine->set_diamond_location(temp_x, temp_y);
	
	mine_in.close();
}
