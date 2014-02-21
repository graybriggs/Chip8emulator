

#include "chip8.h"

void chip8_init(chip8cpu* c8cpu)
{
  c8cpu->program_counter = 0x00;
  c8cpu->I = 0x00;
  
  stack_init(&c8cpu->stack);

  memset(c8cpu->main_memory, 0x00, 4096);
  memset(c8cpu->reg, 0x00, 16);

  init_sprite_data(c8cpu);
}


void chip8_load_resources(chip8cpu* c8cpu, video* v, input* in) {

  c8cpu->p_video = v;
  c8cpu->p_input = in;
}

void parse_instruction(chip8cpu* c8cpu, unsigned short opcode)
{
    switch(opcode & 0xF000) {
      // ignored
    case 0x0000: {
      // ignored

      // ---- 00E0 - Clears the screen -----
      if ((opcode & 0x00F0) == 0x00E0) {
      	clear_screen(c8cpu->p_video);
      	c8cpu->program_counter += 2;
      }
      // ---- 00EE - returns from subroutine -----
      else if((opcode & 0x00FF) == 0x00EE) {
      	c8cpu->program_counter = stack_top(c8cpu->stack);
      	stack_pop(&c8cpu->stack);
      	c8cpu->program_counter += 2;
      }
    }
    
      // ---- 1NNN - Jumps to address NNN
    case 0x1000:
      c8cpu->program_counter = opcode & 0x0FFF;
      c8cpu->program_counter += 2;
      break;
    
    // ---- 2NNN - call subroutine at NNN
    case 0x2000:
      stack_push(&c8cpu->stack, c8cpu->program_counter);
      c8cpu->program_counter = opcode & 0x0FFF;
      break;

      // ---- 3XNN - Skips the next instruction if VX equals NN
    case 0x3000:
      if (c8cpu->reg[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
	     c8cpu->program_counter += 4;
      else
	     c8cpu->program_counter += 2;
      break;

      // ---- 4XNN - Skips the next instruction if VX doesn't erqual NN
    case 0x4000:
      if (c8cpu->reg[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
	     c8cpu->program_counter += 4;
      else
	     c8cpu->program_counter += 2;

      break;

      // ---- 5XY0 - Skips the next instruction if VX equals VY
    case 0x5000:
      if (c8cpu->reg[(opcode & 0x0F00) >> 8] == c8cpu->reg[(opcode & 0x00F0) >> 4])
	     c8cpu->program_counter += 4;
      else
	     c8cpu->program_counter += 2;
      break;

    // ---- 6XNN - Set VX to NN
    case 0x6000:
      c8cpu->reg[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      c8cpu->program_counter += 2;
      break;

      // ---- 7XNN - Adds NN to VX
    case 0x7000:
      c8cpu->reg[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      c8cpu->program_counter += 2;
      break;

    case 0x8000: {
      switch (opcode & 0x000F) {
     
	// ---- 8XY0 - Sets VX to the value of VY
      case 0x0000:
	     c8cpu->reg[(opcode & 0x0F00) >> 8] = c8cpu->reg[(opcode & 0x00F0) >> 4];
	     c8cpu->program_counter += 2;
	     break;

	// ----- 8XY1 - Sets VX to VX | VY
      case 0x0001:
      	c8cpu->reg[(opcode & 0x0F00) >> 8] |= c8cpu->reg[(opcode & 0x00F0) >> 4];
      	c8cpu->program_counter += 2;
      	break;

	// ----- 8XY2 - Sets VX to VX & VY
      case 0x0002:
      	c8cpu->reg[(opcode & 0x0F00) >> 8] &= c8cpu->reg[(opcode & 0x00F0) >> 4];
      	c8cpu->program_counter += 2;
        break;

	// ----- 8XY3 - Sets VX to VX ^ VY
      case 0x0003:
      	c8cpu->reg[(opcode & 0x0F00) >> 8] ^= c8cpu->reg[(opcode & 0x00F0) >> 4];
      	c8cpu->program_counter += 2;
        break;

	// ----- 8XY4 ------
	// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
      case 0x0004:
      	c8cpu->reg[opcode & 0x00F0] += c8cpu->reg[opcode & 0x0F00];
      	if ((opcode & 0x0F00) + (opcode & 0x00F0) > 0xFF)
      	  c8cpu->reg[0x000F] = 0x1;
      	else
      	  c8cpu->reg[0x000F] = 0x0;
      	break;

	// ----- 8XY5 - VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
      case 0x0005: {
        // extract registers
        unsigned char rx = (opcode & 0x0F00) >> 8;
        unsigned char ry = (opcode & 0x00F0) >> 4;
        // get register's values
      	unsigned char vx = c8cpu->reg[rx]; 
      	unsigned char vy = c8cpu->reg[ry];

        // if borrow set register 16 to 1
      	if (vx - vy < 0)
      	  c8cpu->reg[15] = 0x01;

      	c8cpu->reg[rx] = c8cpu->reg[rx] - c8cpu->reg[ry];
      	c8cpu->program_counter += 2;
      	break;
      }
       // ----- 8XY6 - Shifts VX right by one. VF is set to the value of the LSB of VX before the shift
      case 0x0006:
      	c8cpu->reg[0x000F] = (c8cpu->reg[opcode & 0x0F00] & 0x0001); // set V[F] to LSB
      	c8cpu->reg[(opcode & 0x0F00) >> 8] >>= 1;  // do the actual shift
      	c8cpu->program_counter += 2;
      	break;

      // ----- 8XY7 -----
      case 0x0007:
      	break;

      // ----- 8XYE - Shifts VX left by one. VF is set to the value of the MSB of VX before the shift
      case 0x000E:
      	c8cpu->reg[0x000F] = (c8cpu->reg[(opcode & 0x0F00) >> 8] & 0x8000);
      	c8cpu->reg[(opcode & 0x0F00) >> 8] >>= 1;  // do the shift
      
	       break;
      default:
	     break;

      }
    }

      // ---- 9XY0 - Skips the next instruction of VX doesn't equal VY
    case 0x9000:
      if (c8cpu->reg[(opcode & 0x0F00) >> 8] != c8cpu->reg[(opcode & 0x00F0) >> 4])
	     c8cpu->program_counter += 4;
      else
	     c8cpu->program_counter += 2;
      break;

      // ---- ANNN - Sets I to the address NNN
    case 0xA000:
      c8cpu->I = (opcode & 0x0FFF);
      c8cpu->program_counter += 2;
      break;

      // ---- BNNN - Jumps to the address NNN plus V0
    case 0xB000:
      c8cpu->program_counter = (opcode & 0x0FFF) + c8cpu->reg[0x0000];
      break;

      // ---- CXNN - Sets VX to a random number and NN
    case 0xC000:
      c8cpu->reg[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
	     c8cpu->program_counter += 2;
      break;


      // ---- DXYN - Draw sprite at coordinates (VX, VY)
    case 0xD000: {// CHECK THIS!!!!!! 
      // braces for variable scope
      unsigned char vx = c8cpu->reg[(opcode & 0x0F00) >> 8];
      unsigned char vy = c8cpu->reg[(opcode & 0x00F0) >> 4];
      draw_sprite(c8cpu->p_video, vx, vy);  // questionable arguments if decoupling is desired
      c8cpu->program_counter += 2;
      break;
    }
    case 0xE000:
      // ---- EX9E - Skips the next instruction if the key stored in VX is pressed
      if ((opcode & 0x00FF) == 0x009E) {
      
      	if (get_key_pressed(c8cpu->p_input) == c8cpu->reg[(opcode & 0x0F00) >> 8])
      	  c8cpu->program_counter += 4;
      	else
      	  c8cpu->program_counter += 2;
      
      }

      // ---- EXA1 - Skips the next instruction if the key stored in VX isn't pressed
      else if ((opcode & 0x00FF) == 0x00A1) {
	     if ((opcode & 0x00FF) == 0x009E) {
        
	      if (get_key_pressed(c8cpu->p_input) != c8cpu->reg[(opcode & 0x0F00) >> 8])  ///INPUT
    	    c8cpu->program_counter += 4;
    	  else
    	    c8cpu->program_counter += 2;
        
	     }
      }
      
    case 0xF000: {
      printf("hi? %X\n",opcode);
      switch (opcode & 0x00FF) {

        printf("now: %X\n", opcode & 0x00FF);

	// ---- FX07 - Sets VX to the value of the delay timer
      case 0x0007:
      	set_delay_timer(&c8cpu->timer, c8cpu->reg[(opcode & 0x0F00) >> 8]);
      	c8cpu->program_counter += 2;
      	break;

	// ---- FX0A - A key press is awaited and then stored in VX
      case 0x000A: {
        char key = wait_key_press(c8cpu->p_input);  /// INPUT
      	c8cpu->reg[(opcode & 0x0F00) >> 8] = key;
      	c8cpu->program_counter += 2;
      	break;
      }
      
      
        // ---- FX15 - Sets the delay timer to VX
      case 0x0015:
      	set_delay_timer(&c8cpu->timer, c8cpu->reg[(opcode & 0x0F00) >> 8]);
      	c8cpu->program_counter += 2;
      	break;
	
	// ---- FX18 - Sets the sound timer to VX
      case 0x0018:
      	set_sound_timer(&c8cpu->sound_timer, c8cpu->reg[(opcode & 0x0F00) >> 8]);
      	c8cpu->program_counter += 2;
      	break;

	// ---- FX1E - Adds VX to I
      case 0x001E:
      	c8cpu->I += c8cpu->reg[(opcode & 0x0F00) >> 8];
      	c8cpu->program_counter += 2;
      	break;

	// ---- FX29 - Sets I to the location of the sprite for the character in VX. Character 0-F (in hex) are represented by 4x5 font.
      case 0x0029:
      	c8cpu->I = get_sprite_at(c8cpu->reg[(opcode & 0x0F00) >> 8]);
      	c8cpu->program_counter += 2;
      	break;

      case 0x0033: {
      	char contents = c8cpu->reg[(opcode & 0x0F00) >> 8];
      	c8cpu->main_memory[c8cpu->I]     = contents / 100;  // hundreds
      	c8cpu->main_memory[c8cpu->I + 1] = ((contents / 10) % 10); // tens
      	c8cpu->main_memory[c8cpu->I + 2] = contents % 10;
      	
      	c8cpu->program_counter += 2;
      	break;
      }

	// ---- FX55 - Stores V0 to Vx in memory starting at address I
      case 0x0055: {
      	char max_reg55 = (opcode & 0x0F00) >> 8;

        unsigned short mem_ptr = c8cpu->I;
        for (int i = 0; i <= max_reg55; ++i) {
          c8cpu->main_memory[mem_ptr] = c8cpu->reg[i];
          mem_ptr += 1;
        }

      	c8cpu->program_counter += 2;
        break;
      }
	// ---- FX65 - Fills V0 to VX with values from memory starting at address I
      case 0x0065: {
  	   char max_reg65 = (char)opcode & 0x0F00;

        memcpy(c8cpu->reg, c8cpu->main_memory, (size_t)max_reg65);
      	c8cpu->program_counter += 2;
    	 break;
      }
    }
    }
  }
}

/* The first 0x1FF areas of memory in the Chip8 are reserved for
   the interpreter. Part of this memory is used to store the 16
   characters 0-9, A-F. 
*/

void init_sprite_data(chip8cpu* c8cpu)
{
  unsigned int mem_offset = 0;

  // 16 characters
  for (size_t i = 0; i < 16; ++i) {

    char sprite[8 * 5];

    switch (i) {
    case 0x00: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0x90;
      sprite[3] = 0x90;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x01: {
      sprite[0] = 0x20;
      sprite[1] = 0x60;
      sprite[2] = 0x20;
      sprite[3] = 0x20;
      sprite[4] = 0x70;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x02: {
      sprite[0] = 0xF0;
      sprite[1] = 0x10;
      sprite[2] = 0xF0;
      sprite[3] = 0x80;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x03: {
      sprite[0] = 0xF0;
      sprite[1] = 0x10;
      sprite[2] = 0xF0;
      sprite[3] = 0x10;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x04: {
      sprite[0] = 0x90;
      sprite[1] = 0x90;
      sprite[2] = 0xF0;
      sprite[3] = 0x10;
      sprite[4] = 0x10;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x05: {
      sprite[0] = 0xF0;
      sprite[1] = 0x10;
      sprite[2] = 0xF0;
      sprite[3] = 0x10;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x06: {
      sprite[0] = 0xF0;
      sprite[1] = 0x80;
      sprite[2] = 0xF0;
      sprite[3] = 0x90;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x07: {
      sprite[0] = 0xF0;
      sprite[1] = 0x10;
      sprite[2] = 0x20;
      sprite[3] = 0x40;
      sprite[4] = 0x40;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x08: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0xF0;
      sprite[3] = 0x90;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x09: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0xF0;
      sprite[3] = 0x10;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x0A: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0xF0;
      sprite[3] = 0x90;
      sprite[4] = 0x90;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x0B: {
      sprite[0] = 0xE0;
      sprite[1] = 0x90;
      sprite[2] = 0xE0;
      sprite[3] = 0x90;
      sprite[4] = 0xE0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x0C: {
      sprite[0] = 0xF0;
      sprite[1] = 0x80;
      sprite[2] = 0x80;
      sprite[3] = 0x80;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x0D: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0x90;
      sprite[3] = 0x90;
      sprite[4] = 0x90;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x0E: {
      sprite[0] = 0xF0;
      sprite[1] = 0x80;
      sprite[2] = 0xF0;
      sprite[3] = 0x80;
      sprite[4] = 0xF0;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }
    case 0x0F: {
      sprite[0] = 0xF0;
      sprite[1] = 0x80;
      sprite[2] = 0xF0;
      sprite[3] = 0x80;
      sprite[4] = 0x80;
    
      add_to_memory(c8cpu, sprite, mem_offset);
      mem_offset += 8 * 5;
      break;
    }      
    default:
      break;
    }
  }
}


static void add_to_memory(chip8cpu* c8cpu, char* sprite, unsigned int mem_offset_pos)
{
  for (size_t i = 0; i < 8 * 5; ++i) {
    c8cpu->main_memory[mem_offset_pos] = sprite[i];
  }
}


//---------------------

int load_program()
{
  FILE* f = fopen("program.dat", "r");
  if (f == NULL) {
    fprintf(stderr, "Failed to load the program\n");
    return -1;
  }
  fseek(f, 0, SEEK_END);
  size_t f_size = ftell(f);
  rewind(f);
}

