# chip8_interpreter
Fun project which aims to make an emulator* (at the moment its an interpreter) for games developed on chip8. 
Current implementation based on the the original CHIP-8 interpreter created for the RCA COSMAC VIP by Joseph Weisbecker.

## Resources being used:  
https://chip-8.github.io/extensions/#chip-8
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM   
http://www.cs.columbia.edu/~sedwards/classes/2016/4840-spring/designs/Chip8.pdf  
https://en.wikipedia.org/wiki/CHIP-8#CHIP-8_today

09/01/2023:
Under development atm
TODO:  
- [x] basic outline
- [x] dealing with the different opcodes (original implementation)
- [x] ability to take in keyboard input 
- [x] loading in game
- [ ] support for drawing on a gui
- [ ] support for buzzer noise
- [ ] error conditions to check for invalid programs/instructions
- [x] better build system
- [ ] ability to choose different games + separate emulator gui
- [ ] support for chip-48 and schip
- [ ] refactoring/optimizations (use of function pointers)
- [ ] cross-platform support (linux only first)

## How to use
1. Put desired chip8 file to run into the `files` folder.
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
$ sudo apt install llvm clang clang-format cmake build-essential 
```
