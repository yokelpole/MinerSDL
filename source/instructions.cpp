/*
 instructions.cpp
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

#include "instructions.h"

// General SDL Includes.
#include "SDL/SDL.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"

// Functions to access graphical stuff.
#include "sdl_functions.h"

// General function to bring up the instructions.
void display_instructions(SDL_Objects *sdl)
{
    Instructions_Objects instructions_screen;
    
    instructions_screen.update_instructions_graphic(sdl);
    
    instructions_screen.wait_for_keypress(sdl);
}

Instructions_Objects::Instructions_Objects()
{
    instructions_graphic = IMG_Load("./Graphics/instructions/instructions.png");
}

Instructions_Objects::~Instructions_Objects()
{
    SDL_FreeSurface(instructions_graphic);
}

// Update the screen with the instructions graphic.
void Instructions_Objects::update_instructions_graphic(SDL_Objects *sdl)
{
    sdl->apply_surface(0, 0, instructions_graphic, sdl->return_screen());
    
    SDL_Flip(sdl->return_screen());
}

// Wait for user keypress to leave the instructions screen.
void Instructions_Objects::wait_for_keypress(SDL_Objects *sdl)
{
	// Function waits for player to press enter key before progressing.
	bool stop_loop = false;
	
	SDL_Event user_input;
	Timer keyboard_timer;
	
	// Stop a keypress from earlier being carried over.
	SDL_Delay(sdl->KEYPRESS_WAIT);
	
	while(!stop_loop)
	{		
		while(SDL_PollEvent(&user_input))
		{	
			// Quit if the user chooses to close the window.
			if(user_input.type == SDL_QUIT)
			{
				sdl->quit_sdl();
				return;
			}
			else if(user_input.type == SDL_KEYDOWN)
			{
				stop_loop = true;
			}
			else if(user_input.type == SDL_KEYUP)
			{
				keyboard_timer.stop_timer();
			}
		}
        
		SDL_Delay(sdl->SDL_WAIT);
	}
	
	SDL_Delay(sdl->ENTER_WAIT);    
}