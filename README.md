# chip8_interpreter
Fun project which aims to make an emulator* (at the moment its an interpreter) for games developed on chip8. 
Current implementation based on the the original CHIP-8 interpreter created for the RCA COSMAC VIP by Joseph Weisbecker.

Resources being used:  
https://chip-8.github.io/extensions/#chip-8
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM   
http://www.cs.columbia.edu/~sedwards/classes/2016/4840-spring/designs/Chip8.pdf  
https://en.wikipedia.org/wiki/CHIP-8#CHIP-8_today

07/01/2023:
Under development atm
TODO:  
- [x] basic outline
- [x] dealing with the different opcodes (original implementation)
- [ ] ability to take in keyboard input
- [ ] loading in game
- [ ] support for drawing on a gui
- [ ] support for buzzer noise
- [ ] error conditions to check for invalid programs/instructions
- [ ] better build system
- [ ] ability to choose different games + separate emulator gui
- [ ] support for chip-48 and schip
- [ ] refactoring/optimizations (use of function pointers)
