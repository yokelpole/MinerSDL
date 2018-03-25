/*
 save_load.h
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

#ifndef SAVENLOAD
#define SAVENLOAD

#include <iostream>
#include <fstream>

#include "classes.h"

void save_game(MineData *mine, PlayerData *player);

void load_game(MineData *mine, PlayerData *player);

#endif
