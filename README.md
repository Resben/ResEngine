# AEngine by ACHO Interactive
## The Team
- Christien Alden
- Geoff Candy
- Ben Hawkins
- Lane O'Rafferty

## Prerequisites
- Git LFS installed and setup, see https://git-lfs.com/
- CMake version 3.26 or later, this has been bundled with the repo via git-lfs

## Building
1. When first checking out branch, the submodules will need to be initialised
	- Run `git submodule update --init`
	- This will clone remote repositories into the correct place
1. If the project has not been generated, run `GenerateProjects.bat` to generate a solution for Visual Studio 2019
1. Navigate to **AEngine-Development.sln** in the `build` directory
1. Choose either the **Debug** or **Release** configuration to build

## Running
The project can either be run through visual studio debugger or by navigating to the `executable` directory in the root folder, then to the desired configuration and running **AEngine-Demo.exe**; note, the executable must be run from the same directory.
