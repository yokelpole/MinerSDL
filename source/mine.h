/*
 mine.h
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
 Main file for the mining section of the game.
 
 This file accepts the keyboard input for what the player does within
 the mine alongside various other things.
 
 For all graphical work in the mine, see "sdl_functions"
*/

#ifndef MINE
#define MINE

class PlayerData;
class SDL_Objects;
class MineData;

// The main function for the mine.
void mine_function(PlayerData *player, SDL_Objects *sdl, MineData *mine);

// Display the map and where the player has explored.
void mine_show_map(MineData *mine, SDL_Objects *sdl, PlayerData *player);

// Function that waits for user keypress
void wait_for_keypress(SDL_Objects *sdl);

// Move the elevator to the lowest level explored if the player is within
// the elevator.
bool move_elevator_to_bottom(SDL_Objects *sdl, MineData *mine, PlayerData *player);

// Move the elevator to the very top level provided the player is within
// the elevator.
bool move_elevator_to_top(SDL_Objects *sdl, MineData *mine, PlayerData *player);

#endif
