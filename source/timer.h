/*
 timer.h
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

#ifndef TIMER_CLASS
#define TIMER_CLASS

#include "SDL/SDL.h"

class Timer
{
	private:
		int start_time;			// Stores beginning time.
		int current_time;		// Time that is used to compare to start_time.
		int reference_time;		// Time that is stored as a reference to current_time.
		
		bool active;
	public:
		Timer();
	
		bool is_timer_active();		// Checks to see if the timer is currently running.
		
		void begin_timer();			// Start the timer.
		void stop_timer();			// Stop and reset the timer.
		void set_reference_time();	// Set the reference timer against the start timer.
		void reset_timer();			// Reset the timer but keep it running.
		
		
		int check_timer();			// Returns the difference between start_time and current_time
		int check_reference_timer(); 	// Returns difference between current_time and start_time
};

#endif
