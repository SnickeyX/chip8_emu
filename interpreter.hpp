#include <cstdint>

// an emulator class for chip8
class Chip8
{
public:
	Chip8();
	~Chip8();

	void initialise();
	void loadGame(const char *filename);
	void emulateCycle();
	bool drawFlag();
	void setKeys();

private:
	// memory model for chip8
	uint8_t memory[4096];
	// registers
	uint8_t V[16]; // 16 8-bit general purpose registers
	uint16_t I;		 // I register to store memory addresses, only lower 12 bits are used
	// special registers
	uint16_t pc; // program counter
	uint8_t sp;	 // stack pointer
	// active when nonzero, in which case subtract 1 from delay_timer at 60Hz
	// when delay_timer reaches 0, it deactivates
	uint8_t delay_timer;
	// active when nonzero, in which case subtract 1 from sound_timer at 60Hz
	// when sound_timer reaches 0, it deactivates
	// as long as value > 0, sound chip8 buzzer until it reaches 0
	uint8_t sound_timer;
	// stack
	uint16_t stack[12]; // stack
	// keyboard
	uint8_t keyboard[16]; // 16 keys
	// graphics
	uint8_t graphics[64 * 32]; // 64x32 pixel monochrome display
	// To be put anywhere in the first 512 bytes of memory, where the original interpreter was located
	// I'll go with the first 80 bytes from the bottom
	uint8_t chip8_fontset[80] =
			{
					0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
					0x20, 0x60, 0x20, 0x20, 0x70, // 1
					0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
					0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
					0x90, 0x90, 0xF0, 0x10, 0x10, // 4
					0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
					0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
					0xF0, 0x10, 0x20, 0x40, 0x40, // 7
					0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
					0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
					0xF0, 0x90, 0xF0, 0x90, 0x90, // A
					0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
					0xF0, 0x80, 0x80, 0x80, 0xF0, // C
					0xE0, 0x90, 0x90, 0x90, 0xE0, // D
					0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
					0xF0, 0x80, 0xF0, 0x80, 0x80	// F
	};
};