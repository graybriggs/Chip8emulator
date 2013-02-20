

#include "chip8.h"


void parse_instruction(unsigned short opcode)
{
    switch(opcode & 0xF000) {
      // ignored
    case 0x0000: {
      // ignored

      // ---- 00E0 - Clears the screen -----
      if ((opcode & 0x00F0) == 0x00E0) {
	clear_screen(video);
	program_counter += 2;
      }
      // ---- 00EE - returns from subroutine -----
      else if((opcode & 0x00FF) == 0x00EE) {
	program_counter = stack_top(callstack);
	pop(callstack);
	program_counter += 2;
      }
    }
    
      // ---- 1NNN - Jumps to address NNN
    case 0x1000:
      program_counter = opcode & 0x0FFF;
      program_coutner += 2;
      break;
    
    // ---- 2NNN - call subroutine at NNN
    case 0x2000:
      stack_push(callstack, program_counter);
      program_counter = opcode & 0x0FFF;
      break;

      // ---- 3XNN - Skips the next instruction if VX equals NN
    case 0x3000:
      if (reg[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
	program_counter += 4;
      else
	program_counter += 2;
      break;

      // ---- 4XNN - Skips the next instruction if VX doesn't erqual NN
    case 0x4000:
      if (reg[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
	program_counter += 4;
      else
	program_counter += 2;

      break;

      // ---- 5XY0 - Skips the next instruction if VX equals VY
    case 0x5000:
      if (reg[(opcode & 0x0F00) >> 8] == reg[(opcode & 0x00F0) >> 4])
	program_counter += 4;
      else
	program_counter += 2;
      break;

    // ---- 6XNN - Set VX to NN
    case 0x6000:
      reg[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      program_counter += 2;
      break;

      // ---- 7XNN - Adds NN to VX
    case 0x7000:
      reg[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      program_counter += 2;
      break;

    case 0x8000: {
      switch (opcode | 0x000F) {
     
	// ---- 8XY0 - Sets VX to the value of VY
      case 0x0000:
	reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x00F0) >> 4];
	program_counter += 2;
	break;

	// ----- 8XY1 - Sets VX to VX | VY
      case 0x0001:
	reg[(opcode & 0x0F00) >> 8] |= reg[(opcode & 0x00F0) >> 4];
	program_counter += 2;
	break;

	// ----- 8XY2 - Sets VX to VX & VY
      case 0x0002:
	reg[(opcode & 0x0F00) >> 8] &= reg[(opcode & 0x00F0) >> 4];
	program_counter += 2;
        break;

	// ----- 8XY3 - Sets VX to VX ^ VY
      case 0x0003:
	reg[(opcode & 0x0F00) >> 8] ^= reg[(opcode & 0x00F0) >> 4];
	program_counter += 2;
        break;

	// ----- 8XY4 ------
	// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
      case 0x0004:
	reg[opcode & 0x00F0] += reg[opcode & 0x0F00];
	if ((opcode & 0x0F00) + (opcode & 0x00F0) > 0xFF)
	  reg[0x000F] = 0x1;
	else
	  reg[0x000F] = 0x0;
	break;

	// ----- 8XY5 - VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
      case 0x0005:
	unsigned short vx = reg[(opcode & 0x0F00) >> 8]; 
	unsigned short vy = reg[(opcode & 0x00F0) >> 4];

	if (vy - vx < 0x0000)
	  reg[0x000F] = 0x0001;

	reg[vx] -= reg[vx];
	program_counter += 2;
	break;

       // ----- 8XY6 - Shifts VX right by one. VF is set to the value of the LSB of VX before the shift
      case 0x0006:
	reg[0x000F] = (reg[opcode & 0x0F00] & 0x0001); // set V[F] to LSB
	reg[(opcode & 0x0F00) >> 8] >> 1;
	program_counter += 2;
	break;

      // ----- 8XY7 -----
      case 0x0007:
	break;

      // ----- 8XYE - Shifts VX left by one. VF is set to the value of the MSB of VX before the shift
      case 0x000E:
	reg[0x000F] = (reg[(opcode & 0x0F00) >> 8] & 0x8000);
	reg[(opcode & 0x0F00) >> 8] >> 1;
      
	break;
      default:
	  break;

      }
    }

      // ---- 9XY0 - Skips the next instruction of VX doesn't equal VY
    case 0x9000:
      if (reg[(opcode & 0x0F00) >> 8] != reg[(opcode & 0x00F0) >> 4])
	program_counter += 4;
      else
	program_counter += 2;
      break;

      // ---- ANNN - Sets I to the address NNN
    case 0xA000:
      I = (opcode & 0x0FFF);
      program_counter += 2;
      break;

      // ---- BNNN - Jumps to the address NNN plus V0
    case 0xB000:
      program_counter = (opcode & 0x0FFF) + reg[0x0000];
      break;

      // ---- CXNN - Sets VX to a random number and NN
    case 0xC000:
      reg[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF)
	program_counter += 2;
      break;


      // ---- DXYN -
    case 0xD000:
      break;

    case 0xE000:
      // ---- EX9E - Skips the next instruction if the key stored in VX is pressed
      if ((opcode & 0x00FF) == 0x009E) {
      }

      // ---- EXA1 - Skips the next instruction if the key stored in VX isn't pressed
      else if ((opcode & 0x00FF) == 0x00A1) {

      }
      
    case 0xF000: {

      switch(opcode & 0x00FF) {
      
	// ---- FX07 - Sets VX to the value of the delay timer
      case 0x0007:
	set_delay_timer(timer, reg[opcode & 0x0F00]);
	break;

	// ---- FX0A - A key press is awaited and then stored in VX
      case 0x000A:
	char key = wait_key_press();
	reg[(opcode & 0x0F00) >> 8] = key;
	program_counter += 2;
	break;
      }
      
        // ---- FX15 - Sets the delay timer to VX
      case 0x0015:
	set_delay_timer(reg[opcode & 0x0F00]);
	program_counter += 2;
	break;
	
	// ---- FX18 - Sets the sound timer to VX
      case 0x0018:
	set_sound_timer(reg[opcode & 0x0F00]);
	program_counter += 2;
	break;

	// ---- FX1E - Adds VX to I
      case 0x001E:
	I += reg[opcode & 0x0F00];
	program_counter += 2;
	break;

	// ---- FX55 - Stores V0 to Vx in memory starting at address I
      case 0x0055:
	char max_reg = opcode & 0x0F00;

	for (int i = 0; i <= max_reg; i++) {
	  main_memory[i] = reg[i];
	}
	program_counter += 2;

	// ---- FX65 - Fills V0 to VX with values from memory starting at address I
      case 0x0065:
	char max_reg = opcode & 0x0F00;

	for (int i = 0; <= max_reg; i++) {
	  reg[i] = main_memory[I++];
	}
	program_counter += 2;
	break;
    }
  
  }
}

