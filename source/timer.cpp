/*
 timer.cpp
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
 Provides timer based functionality to programs that use SDL.
*/

#include "SDL/SDL.h"

#include "timer.h"

Timer::Timer()
{
	// Set all variables to zero.
	start_time = 0;
	current_time = 0;
	reference_time = 0;
	
	// Timer is not active at time of creation.
	active = false;
}

// Return 
bool Timer::is_timer_active()
{
	return active;
}

// Start the timer.
void Timer::begin_timer()
{
	start_time = SDL_GetTicks();
	active = true;
}

// Stop and reset the timer.
void Timer::stop_timer()
{
	start_time = 0;
	current_time = 0;
	reference_time = 0;
	active = false;
}

// Set the reference timer aga#include <string>inst the start timer.
void Timer::set_reference_time()
{
	reference_time = SDL_GetTicks();
}

// Reset the timer but keep it running.
void Timer::reset_timer()
{
	start_time = SDL_GetTicks();
	reference_time = 0;
}

// Check the current time since the clock has been started.
int Timer::check_timer()
{
	current_time = SDL_GetTicks() - start_time;
	
	return current_time;
}

// Check the reference timer against the start timer.
int Timer::check_reference_timer()
{
	int temp;
	
	temp = SDL_GetTicks() - start_time;
	temp = reference_time - temp;
	
	return temp;
}
