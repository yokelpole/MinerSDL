/*
 change_working_directory.cpp
 ---------------------------------------
 This file is a part of MinerSDL.
 * This code was retrieved from a posting on stackoverflow *
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
 This file allows a user on MacOS to package and compile the program
 by setting the working directory to the proper location.
*/
#include "change_working_directory.h"

// Only compile this if an Apple computer is being used.
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#include <unistd.h> 

void change_directory_macos()
{  
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(path);

}
#endif