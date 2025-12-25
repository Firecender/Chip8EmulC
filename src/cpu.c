#include "../Inc/cpu.h"
#include "../Inc/pixel.h"
#include <SDL3/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>
CPU cpu;
opTable table;

void initCpu() {
  cpu.PC = pointerStart;
  cpu.stackPointeur = 0;
  cpu.i = 0;

  for (int i = 0; i < memorySize; i++) {
    cpu.memory[i] = 0;
  }
  for (int i = 0; i < 16; i++) {
    cpu.V[i] = 0;
    cpu.stack[i] = 0;
  }

  cpu.timerjeu = 0;
  cpu.timerson = 0;
}
void initopTable() {
  table.code[0] = 0x0FFF;
  table.mask[0] = 0xFFFF; // 0NNN
  table.code[1] = 0x00E0;
  table.mask[1] = 0XFFFF; // 00E0
  table.code[2] = 0x00EE;
  table.mask[2] = 0xFFFF; // 00EE
  table.code[3] = 0x1000;
  table.mask[3] = 0xF000; // 1NNN
  table.code[4] = 0x2000;
  table.mask[4] = 0xF000; // 2NNN
  table.code[5] = 0x3000;
  table.mask[5] = 0xF000; // 3NNN
  table.code[6] = 0x4000;
  table.mask[6] = 0xF000; // 4NNN
  table.code[7] = 0x5000;
  table.mask[7] = 0xF00F; // 5XY0
  table.code[8] = 0x6000;
  table.mask[8] = 0xF000; // 6XNN
  table.code[9] = 0x7000;
  table.mask[9] = 0xF000; // 7XNN
  table.code[10] = 0x8000;
  table.mask[10] = 0xF00F; // 8XY0
  table.code[11] = 0x8001;
  table.mask[11] = 0xF001; // 8XY1
  table.code[12] = 0x8002;
  table.mask[11] = 0xF002; // 8XY2
  table.code[13] = 0x8003;
  table.mask[13] = 0xF003; // 8XY3
  table.code[14] = 0x8004;
  table.mask[14] = 0xF004; // 8XY4
  table.code[15] = 0x8005;
  table.mask[15] = 0xF005; // 8XY5
  table.code[16] = 0x8006;
  table.mask[16] = 0xF006; // 8XY6
  table.code[17] = 0x8007;
  table.mask[17] = 0xF007; // 8XY7
  table.code[18] = 0x800E;
  table.mask[18] = 0xF003; // 8XYE
  table.code[19] = 0x9000;
  table.mask[19] = 0xF000; // 9XY0
  table.code[20] = 0xA000;
  table.mask[20] = 0xF000; // ANNN
  table.code[21] = 0xB000;
  table.mask[21] = 0xF000; // BNNN
  table.code[22] = 0xC000;
  table.mask[21] = 0xF000; // CXNN
  table.code[23] = 0xD000;
  table.mask[23] = 0xF000; // DXYN
  table.code[24] = 0xE09E;
  table.mask[24] = 0xF0FF; // EX9E
  table.code[25] = 0xE0A1;
  table.mask[25] = 0xF0FF; // EXA1
  table.code[26] = 0xF007;
  table.mask[26] = 0xF0FF; // FX07
  table.code[27] = 0xF00A;
  table.mask[27] = 0xF0FF; // FX0A
  table.code[28] = 0xF015;
  table.mask[28] = 0xF0FF; // FX15
  table.code[29] = 0x8018;
  table.mask[29] = 0xF0FF; // FX18
  table.code[30] = 0xF01E;
  table.mask[30] = 0xF0FF; // FX1E
  table.code[31] = 0xF029;
  table.mask[31] = 0xF0FF; // FX29
  table.code[32] = 0xF033;
  table.mask[32] = 0xF0FF; // FX33
  table.code[33] = 0xF055;
  table.mask[33] = 0xF0FF; // FX55
  table.code[34] = 0xF065;
  table.mask[34] = 0xF0FF; // FX65
}

void stripOpCode(Uint16 opCode, Uint8 *b0, Uint8 *b1, Uint8 *b2, Uint8 *b3) {
  *b0 = (opCode) & 0x000F;
  *b1 = (opCode & 0x00F0) >> 4;
  *b2 = (opCode & 0x0F00) >> 8;
  *b3 = (opCode & 0xF000) >> 12;
}

void executeOp(Uint16 opCode) {
  Uint8 b0 = 0;
  Uint8 b1 = 0;
  Uint8 b2 = 0;
  Uint8 b3 = 0;
  /*
   * Seperate the opCode into b vars for parsing
   * 0xAFED --> b0 = 0xD, b1 = 0xE, b2 = 0xF, b3 = 0xA
   */
  stripOpCode(opCode, &b0, &b1, &b2, &b3);

  Uint8 i = 0;
  do {
    if ((opCode & table.mask[i]) == table.code[i]) {
      break;
    }

  } while (i < NBOPCODE + 1);

  if (i > NBOPCODE) {
    printf("Something isn't right ... \n");
    exit(EXIT_FAILURE);
  }

  switch (i) {
  case 1: {
    clearScreen(); // clear the screen
  }
  case 2: { // return;
    //???
  }
  case 3: { // 1NNN goto NNN
    cpu.PC = (((b2 << 4) | b1) << 4 | b0);
  }
  case 4: { // 2NNN
    executeOp(cpu.memory[(((b2 << 4) | b1) << 4 | b0)]);
  }
  case 5: { // 3XNN
    if (cpu.V[b2] == ((b1 << 4) | b0)) {
      cpu.PC += 8;
    }
  }
  case 6: { // 4XNN
    if (cpu.V[b2] != ((b1 << 4) | b0)) {
      cpu.PC += 8;
    }
  }
  case 7: { // 5XY0
    if (cpu.V[b2] != cpu.V[b1]) {
      cpu.PC += 8;
    }
  }
  case 8: { // 6XNN
    cpu.V[b2] = ((b2 << 4) | b1);
  }
  case 9: { // 7XNN
    cpu.V[b2] += ((b2 << 4) | b1);
  }
  case 10: {
  }
  case 11: {
  }
  case 12: {
  }
  case 13: {
  }
  case 14: {
  }
  case 15: {
  }
  case 16: {
  }
  case 17: {
  }
  case 18: {
  }
  case 19: {
  }
  case 20: {
  }
  case 21: {
  }
  case 22: {
  }
  case 23: {
  }
  case 24: {
  }
  case 25: {
  }
  case 26: {
  }
  case 27: {
  }
  case 28: {
  }
  case 29: {
  }
  case 30: {
  }
  case 31: {
  }
  case 32: {
  }
  case 33: {
  }
  case 34: {
  }
  default: {
  }
  }
}

void decompter() {
  if (cpu.timerson > 0) {
    cpu.timerson--;
  }
  if (cpu.timerjeu > 0) {
    cpu.timerjeu--;
  }
}
Uint16 getOpCode() {
  return cpu.memory[cpu.stackPointeur] << 8 | cpu.memory[cpu.stackPointeur + 1];
}
