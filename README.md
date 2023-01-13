![BUILD-SHIELD](https://img.shields.io/github/actions/workflow/status/snickeyx/chip8_emu/build.yml?style=flat-square)
# A CHIP8 Emulator!
Fun project which aims to make an emulator* (at the moment its an interpreter) for games developed on chip8.  
Current implementation based on the the original CHIP-8 interpreter created for the RCA COSMAC VIP by Joseph Weisbecker.

## How to use
Pre-Conditions: You <b>MUST</b> have packages inlcluding llvm, clang, cmake (can get these with build-essential not inlcuding llvm). For GUI purposes, you must also have libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxext-dev libxcursor-dev libxinerama-dev libxi-dev. 
1. Put desired chip8 file to run into the `files` folder. (if you don't have any roms, check [this](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html) out)
2. Create the emulator binary using `make -C build` (use from main directory and not any sub-directory) 
3. Go into the files folder and run the emulator using `./chip8_emulator`
4. Enjoy ٩(˘◡˘)۶

## Troubleshooting
- Currently this can only run on linux systems, however on Windows you can use `wsl` (windows subsystem for linux) to run this program or on a Mac getting a linux VM (a docker running a linux VM is another option). 
- You will at least need CMAKE ver 3.1 (get it using `sudo apt install cmake`). If you have trouble with getting the latest version, check [this](https://stackoverflow.com/questions/49859457/how-to-reinstall-the-latest-cmake-version) thread out.
- If you are missing any libraries or tools, try:
```
$ sudo apt update
$ sudo apt upgrade
$ sudo apt install llvm clang clang-format cmake build-essential libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxext-dev libxcursor-dev libxinerama-dev libxi-dev
```
## Extra features I want to add:  
- [ ] separate emualtor gui
- [ ] ability to choose different games from a separate emulator gui
- [ ] support for chip-48 and s-chip
- [ ] better cross-platform support 

## Resources used:  
https://chip-8.github.io/extensions/#chip-8
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM   
http://www.cs.columbia.edu/~sedwards/classes/2016/4840-spring/designs/Chip8.pdf  
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0nnn
https://en.wikipedia.org/wiki/CHIP-8#CHIP-8_today

Open-Source Libraries Used (in accordance with their license):  
[GLFW](https://github.com/glfw/glfw)      
[GLEW](https://github.com/nigels-com/glew)
