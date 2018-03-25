/*
 instructions.h
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
 A simple function to display the instructions screen.
 */

#ifndef INSTRUCTIONS
#define INSTRUCTIONS

// General SDL Includes.
#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

// Functions to access graphical stuff.
#include "sdl_functions.h"

// General function to bring up the instructions.
void display_instructions(SDL_Objects *sdl);

class Instructions_Objects
{
private:
    SDL_Surface *instructions_graphic;
    
public:
    Instructions_Objects();
    ~Instructions_Objects();
    
    // Update the screen with the instructions graphic.
    void update_instructions_graphic(SDL_Objects *sdl);
    
    // Wait for user keypress to leave the instructions screen.
    void wait_for_keypress(SDL_Objects *sdl);
};


#endif
