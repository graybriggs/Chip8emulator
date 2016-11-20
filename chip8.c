

#include "chip8.h"

#include "opcodes.h"

void chip8_init(chip8cpu* c8cpu) {
  c8cpu->program_counter = PROGRAM_MEMORY_START;
  c8cpu->I = 0x00;
  
  stack_init(&c8cpu->stack, c8cpu);

  memset(c8cpu->main_memory, 0x00, CHIP8_MEMORY_MAX);
  memset(c8cpu->reg, 0x00, NUM_REGISTERS);

  init_sprite_data(c8cpu);
}


void chip8_load_resources(chip8cpu* c8cpu, video* v, input* in) {
  c8cpu->p_video = v;
  c8cpu->p_input = in;
}

void advance_pc(chip8cpu* c8cpu) {
  c8cpu->program_counter += 2;
}

void read_instruction(chip8cpu* c8cpu) {
  return chip8cpu->main_memory[c8cpu->program_counter];
}

void fetch_decode_execute_cycle(chip8cpu* c8cpu) {

  BYTE ins = read_instruction(c8cpu);

  for (;;) {
    advance_pc(c8cpu);
    parse_instruction(c8cpu, ins); // decode and execute happens here
    ins = read_instruction(c8cpu);
  }
}



void parse_instruction(chip8cpu* c8cpu, WORD opcode) {
    switch(opcode & 0xF000) {
      // ignored
    case 0x00: {
      // ignored

      // ---- 00E0 - Clears the screen -----
      if ((opcode & 0x00F0) == OPCODE_CLS) {
      	clear_screen(c8cpu->p_video);
      }
      // ---- 00EE - returns from subroutine -----
      else if((opcode & 0x00FF) == OPCODE_RET) {
      	c8cpu->program_counter = stack_top(c8cpu->stack);
      	stack_pop(&c8cpu->stack);
      }
    }
    
      // ---- 1NNN - Jumps to address NNN
    case OPCODE_JP:
      c8cpu->program_counter = opcode & 0x0FFF;
      advance_pc(c8cpu);
      break;
    
    // ---- 2NNN - call subroutine at NNN
    case OPCODE_CALL:
      stack_push(&c8cpu->stack, &c8cpu, c8cpu->program_counter);
      c8cpu->program_counter = opcode & 0x0FFF;
      break;

      // ---- 3XNN - Skips the next instruction if VX equals NN
    case OPCODE_SKIP_IF_EQUAL:
      if (c8cpu->reg[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
       advance_pc(c8cpu);

      break;

      // ---- 4XNN - Skips the next instruction if VX doesn't erqual NN
    case OPCODE_SKIP_NOT_EQUAL:
      if (c8cpu->reg[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
	      advance_pc(c8cpu);
      break;

      // ---- 5XY0 - Skips the next instruction if VX equals VY
    case OPCODE_SKIP_REG_EQUAL:
      if (c8cpu->reg[(opcode & 0x0F00) >> 8] == c8cpu->reg[(opcode & 0x00F0) >> 4])
	     advance_pc(c8cpu);
      break;

    // ---- 6XNN - Set VX to NN
    case OPCODE_LOAD_REG_BYTE:
      c8cpu->reg[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      break;

      // ---- 7XNN - Adds NN to VX
    case OPCODE_ADD_BYTE_REG:
      c8cpu->reg[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      break;

    case 0x8000: {
      switch (opcode & 0x000F) {
     
	// ---- 8XY0 - Sets VX to the value of VY
      case OPCODE_LOAD_REG_REG:
	     c8cpu->reg[(opcode & 0x0F00) >> 8] = c8cpu->reg[(opcode & 0x00F0) >> 4];
	     break;

	// ----- 8XY1 - Sets VX to VX | VY
      case OPCODE_BITWISE_OR:
      	c8cpu->reg[(opcode & 0x0F00) >> 8] |= c8cpu->reg[(opcode & 0x00F0) >> 4];
      	break;

	// ----- 8XY2 - Sets VX to VX & VY
      case OPCODE_BITWISE_AND:
      	c8cpu->reg[(opcode & 0x0F00) >> 8] &= c8cpu->reg[(opcode & 0x00F0) >> 4];
        break;

	// ----- 8XY3 - Sets VX to VX ^ VY
      case OPCODE_BITWISE_XOR:
      	c8cpu->reg[(opcode & 0x0F00) >> 8] ^= c8cpu->reg[(opcode & 0x00F0) >> 4];
        break;

	// ----- 8XY4 ------
	// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
      case OPCODE_ADD:
      	c8cpu->reg[opcode & 0x00F0] += c8cpu->reg[opcode & 0x0F00];
      	if ((opcode & 0x0F00) + (opcode & 0x00F0) > 0xFF)
      	  c8cpu->reg[0x000F] = 0x1;
      	else
      	  c8cpu->reg[0x000F] = 0x0;
      	break;

	// ----- 8XY5 - VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
      case OPCODE_SUB: {
        // extract registers
        BYTE rx = (opcode & 0x0F00) >> 8;
        BYTE ry = (opcode & 0x00F0) >> 4;
        // get register's values
      	BYTE vx = c8cpu->reg[rx]; 
      	BYTE vy = c8cpu->reg[ry];

        // if borrow set register 16 to 1
      	if (vx - vy < 0)
      	  c8cpu->reg[15] = 0x01;

      	c8cpu->reg[rx] = c8cpu->reg[rx] - c8cpu->reg[ry];
      	break;
      }
       // ----- 8XY6 - Shifts VX right by one. VF is set to the value of the LSB of VX before the shift
      case OPCODE_SHIFT_BY_ONE:
      	c8cpu->reg[0x000F] = (c8cpu->reg[opcode & 0x0F00] & 0x0001); // set V[F] to LSB
      	c8cpu->reg[(opcode & 0x0F00) >> 8] >>= 1;  // do the actual shift
      	break;

      // ----- 8XY7 -----
      case OPCODE_VX_MINUS_VY:
      	break;

      // ----- 8XYE - Shifts VX left by one. VF is set to the value of the MSB of VX before the shift
      case OCODE_SHIFT_ONE_LEFT:
        advance_pc(c8cpu);
      	c8cpu->reg[0x000F] = (c8cpu->reg[(opcode & 0x0F00) >> 8] & 0x8000);
      	c8cpu->reg[(opcode & 0x0F00) >> 8] >>= 1;  // do the shift
	       break;
      default:
	     break;

      }
    }

      // ---- 9XY0 - Skips the next instruction of VX doesn't equal VY
    case OPCODE_SKIP_IF_NOT_EQUAL:
      if (c8cpu->reg[(opcode & 0x0F00) >> 8] != c8cpu->reg[(opcode & 0x00F0) >> 4])
	     advance_pc(c8cpu);
      break;

      // ---- ANNN - Sets I to the address NNN
    case OPCODE_SET_IP:
      c8cpu->I = (opcode & 0x0FFF);
      break;

      // ---- BNNN - Jumps to the address NNN plus V0
    case OPCODE_JUMP_ADDR:
      c8cpu->program_counter = (opcode & 0x0FFF) + c8cpu->reg[0x0000];
      break;

      // ---- CXNN - Sets VX to a random number and NN
    case OPCODE_BITWISE_RAND:
      c8cpu->reg[(opcode & 0x0F00) >> 8] = rand() & (opcode & 0x00FF);
      break;


      // ---- DXYN - Draw sprite from memory location at coordinates (VX, VY)
    case OPCODE_DISP: {// CHECK THIS!!!!!! 
      // braces for variable scope

      // I is assumed to be pointing at specific memory location
      // that contains a sprite

      // copy 5 bytes of 

      unsigned char vx = c8cpu->reg[(opcode & 0x0F00) >> 8];
      unsigned char vy = c8cpu->reg[(opcode & 0x00F0) >> 4];
      draw_sprite(c8cpu->p_video, vx, vy); 
      break;
    }
    case 0xE000:
      // ---- EX9E - Skips the next instruction if the key stored in VX is pressed
      if ((opcode & 0x00FF) == OPCODE_SKIP_IF_KEY) {
      	if (get_key_pressed(c8cpu->p_input) == c8cpu->reg[(opcode & 0x0F00) >> 8]) {
          advance_pc(c8cpu);
        }      
      }

      // ---- EXA1 - Skips the next instruction if the key stored in VX isn't pressed
      else if ((opcode & 0x00FF) == OPCODE_SKIP_NOT_KEY) {
	      if (get_key_pressed(c8cpu->p_input) != c8cpu->reg[(opcode & 0x0F00) >> 8]) { ///INPUT
          advance_pc(c8cpu);
        }        
	     }
      }
      
    case 0xF000: {
      switch (opcode & 0x00FF) {

	// ---- FX07 - Sets VX to the value of the delay timer
      case OPCODE_TIMER:
      	set_delay_timer(&c8cpu->timer, c8cpu->reg[(opcode & 0x0F00) >> 8]);
      	advance_pc(c8cpu);
      	break;

	// ---- FX0A - A key press is awaited and then stored in VX
      case OPCODE_AWAIT_KEY_PRESS: {
        char key = wait_key_press(c8cpu->p_input);  /// INPUT
      	c8cpu->reg[(opcode & 0x0F00) >> 8] = key;
      	break;
      }
      
      
        // ---- FX15 - Sets the delay timer to VX
      case OPCODE_DELAY_TIMER:
      	set_delay_timer(&c8cpu->timer, c8cpu->reg[(opcode & 0x0F00) >> 8]);
      	break;
	
	// ---- FX18 - Sets the sound timer to VX
      case OPCODE_SET_SOUND:
      	set_sound_timer(&c8cpu->sound_timer, c8cpu->reg[(opcode & 0x0F00) >> 8]);
      	break;

	// ---- FX1E - Adds VX to I
      case OPCODE_ADD_TO_IP:
      	c8cpu->I += c8cpu->reg[(opcode & 0x0F00) >> 8];

        /* this range overflow check is apparently an undocumented feature of the Chip8 */
        if (c8cpu->I > 0xFFF) {
            c8cpu->reg[0xF] = 1;
        }
      	break;

	// ---- FX29 - Sets I to the location of the sprite for the character in VX. Character 0-F (in hex) are represented by 4x5 font.
      case OPCODE_SET_I_SPRITE:
      	c8cpu->I = get_sprite_at(c8cpu->reg[(opcode & 0x0F00) >> 8]);
      	break;

      case OPCODE_STORE_BCD: {
      	char contents = c8cpu->reg[(opcode & 0x0F00) >> 8];
      	c8cpu->main_memory[c8cpu->I]     = contents / 100;  // hundreds
      	c8cpu->main_memory[c8cpu->I + 1] = ((contents / 10) % 10); // tens
      	c8cpu->main_memory[c8cpu->I + 2] = contents % 10;
      	break;
      }

	// ---- FX55 - Stores V0 to Vx in memory starting at address I
      case OPCODE_STORE: {
      	BYTE max_reg55 = (opcode & 0x0F00) >> 8;

        WORD mem_ptr = c8cpu->I;
        for (int i = 0; i <= max_reg55; ++i) {
          c8cpu->main_memory[mem_ptr] = c8cpu->reg[i];
          mem_ptr += 1;
        }
        break;
      }
	// ---- FX65 - Fills V0 to VX with values from memory starting at address I
      case OPCODE_FILL: {
  	    BYTE max_reg65 = (BYTE)opcode & 0x0F00;

        memcpy(c8cpu->reg, c8cpu->main_memory, (CHAR)max_reg65);
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

void init_sprite_data(chip8cpu* c8cpu) {
  WORD mem_offset = 0;

  // 16 characters
  for (unsigned i = 0; i < 16; ++i) {

    BYTE sprite[5];

    switch (i) {
    case 0x00: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0x90;
      sprite[3] = 0x90;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x01: {
      sprite[0] = 0x20;
      sprite[1] = 0x60;
      sprite[2] = 0x20;
      sprite[3] = 0x20;
      sprite[4] = 0x70;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x02: {
      sprite[0] = 0xF0;
      sprite[1] = 0x10;
      sprite[2] = 0xF0;
      sprite[3] = 0x80;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      } 
      break;
    }
    case 0x03: {
      sprite[0] = 0xF0;
      sprite[1] = 0x10;
      sprite[2] = 0xF0;
      sprite[3] = 0x10;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x04: {
      sprite[0] = 0x90;
      sprite[1] = 0x90;
      sprite[2] = 0xF0;
      sprite[3] = 0x10;
      sprite[4] = 0x10;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x05: {
      sprite[0] = 0xF0;
      sprite[1] = 0x10;
      sprite[2] = 0xF0;
      sprite[3] = 0x10;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x06: {
      sprite[0] = 0xF0;
      sprite[1] = 0x80;
      sprite[2] = 0xF0;
      sprite[3] = 0x90;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x07: {
      sprite[0] = 0xF0;
      sprite[1] = 0x10;
      sprite[2] = 0x20;
      sprite[3] = 0x40;
      sprite[4] = 0x40;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x08: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0xF0;
      sprite[3] = 0x90;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x09: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0xF0;
      sprite[3] = 0x10;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x0A: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0xF0;
      sprite[3] = 0x90;
      sprite[4] = 0x90;
    
      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x0B: {
      sprite[0] = 0xE0;
      sprite[1] = 0x90;
      sprite[2] = 0xE0;
      sprite[3] = 0x90;
      sprite[4] = 0xE0;
    

      for (size_t i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x0C: {
      sprite[0] = 0xF0;
      sprite[1] = 0x80;
      sprite[2] = 0x80;
      sprite[3] = 0x80;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x0D: {
      sprite[0] = 0xF0;
      sprite[1] = 0x90;
      sprite[2] = 0x90;
      sprite[3] = 0x90;
      sprite[4] = 0x90;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x0E: {
      sprite[0] = 0xF0;
      sprite[1] = 0x80;
      sprite[2] = 0xF0;
      sprite[3] = 0x80;
      sprite[4] = 0xF0;
    

      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }
    case 0x0F: {
      sprite[0] = 0xF0;
      sprite[1] = 0x80;
      sprite[2] = 0xF0;
      sprite[3] = 0x80;
      sprite[4] = 0x80;
    
      for (unsigned i = 0; i < 5; ++i) {
        c8cpu->main_memory[mem_offset++] = sprite[i];
      }
      break;
    }      
    default:
      break;
    }
  }
}


static void add_to_memory(chip8cpu* c8cpu, BYTE* sprite, WORD mem_offset_pos) {
  // 5 bytes of memory
  for (unsigned i = 0; i < 5; ++i) {
    c8cpu->main_memory[mem_offset_pos++] = sprite[i];
  }
}


//---------------------

int load_program(chip8cpu* c8cpu) {
    FILE* f = fopen("program.dat", "r");
    if (f == NULL) {
        fprintf(stderr, "Failed to load the program\n");
        return -1;
    }

    fseek(f, 0, SEEK_END);
    size_t f_size = ftell(f);
    rewind(f);

    BYTE* mem_ptr = c8cpu->main_memory + 0x200;
    printf("allocating %d bytes\n", (int)f_size);
    BYTE* ins_buf = malloc(f_size * 2);

    while (!feof(f)) {
        fread(ins_buf, 1, 4, f);
        printf("%X %X %X %X\n", ins_buf[0], ins_buf[1], ins_buf[2], ins_buf[3]);
    }

    c8cpu->program_counter = 0x200; // starting point in mem


    for (unsigned i = 0; i < f_size * 2; ++i) {
        c8cpu->main_memory[0x200 + i] = ins_buf[i];
    }

}
