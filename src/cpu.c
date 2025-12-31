#include "../Inc/cpu.h"
#include "../Inc/pixel.h"
#include <SDL3/SDL_keyboard.h>
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

  Uint8 units = 0;
  Uint8 tens = 0;
  Uint8 hundreds = 0;
  /*
   * Seperate the opCode into b vars for parsing
   * 0xAFED --> b0 = 0xD, b1 = 0xE, b2 = 0xF, b3 = 0xA
   */
  stripOpCode(opCode, &b0, &b1, &b2, &b3);
  Uint16 nnn = (((b2 << 4) | b1) << 4 | b0);
  Uint8 nn = ((b1 << 4) | b0);

  Uint8 i = 1;
  do {
    if ((opCode & table.mask[i]) == table.code[i]) {
      break;
    }

  } while (i < NBOPCODE + 1);

  if (i > NBOPCODE) {
    printf("Something isn't right ... \n");
    exit(EXIT_FAILURE);
  }
  printf("executing %d\n", i);

  switch (i) {
  case 1: {
    clearScreen(); // clear the screen
    break;
  }
  case 2: { // return;
    cpu.stackPointeur--;
    cpu.PC = cpu.stack[cpu.stackPointeur] - 2;
    break;
  }
  case 3: { // 1NNN goto NNN
    cpu.PC = nnn - 2;
    break;
  }
  case 4: { // 2NNN
    cpu.stack[cpu.stackPointeur] = cpu.PC;
    cpu.stackPointeur++;
    cpu.PC = nnn;
    break;
  }
  case 5: { // 3XNN
    if (cpu.V[b2] == nn) {
      cpu.PC += 2;
    }
    break;
  }
  case 6: { // 4XNN
    if (cpu.V[b2] != nn) {
      cpu.PC += 2;
    }
    break;
  }
  case 7: { // 5XY0
    if (cpu.V[b2] != cpu.V[b1]) {
      cpu.PC += 2;
    }
    break;
  }
  case 8: { // 6XNN
    cpu.V[b2] = nn;
    break;
  }
  case 9: { // 7XNN
    cpu.V[b2] += nn;
    break;
  }
  case 10: { // 8XY0
    cpu.V[b2] = cpu.V[b1];
    break;
  }
  case 11: { // 8XY1
    cpu.V[b2] = cpu.V[b2] | cpu.V[b1];
    break;
  }
  case 12: { // 8XY2
    cpu.V[b2] = cpu.V[b2] & cpu.V[b1];
    break;
  }
  case 13: { // 8XY3
    cpu.V[b2] = cpu.V[b2] ^ cpu.V[b1];
    break;
  }
  case 14: { // 8XY4
    if (cpu.V[b2] + cpu.V[b1] > 0xFF) {
      cpu.V[0xF] = 1;
    }
    cpu.V[b2] += cpu.V[b1];
    break;
  }
  case 15: { // 8XY5
    cpu.V[0xF] = cpu.V[b2] >= cpu.V[b1] ? 1 : 0;
    cpu.V[b2] -= cpu.V[b1];
    break;
  }
  case 16: { // 8XY6  1010 1010 0000 0001
    cpu.V[0xF] = cpu.V[b2] & 0x01;
    cpu.V[b2] = cpu.V[b2] >> 1;
    break;
  }
  case 17: { // 8XY7
    cpu.V[0xF] = cpu.V[b1] >= cpu.V[b2] ? 1 : 0;
    cpu.V[b2] = cpu.V[b1] - cpu.V[b2];
    break;
  }
  case 18: { // 8XYE 1010 1010 1000 0000
    cpu.V[0xF] = (cpu.V[b2] & 0x80) != 0 ? 1 : 0;
    cpu.V[b2] = cpu.V[b2] << 1;
    break;
  }
  case 19: { // 9XY0
    if (cpu.V[b2] != cpu.V[b1]) {
      cpu.PC += 2;
    }
    break;
  }
  case 20: { // ANNN
    cpu.i = nnn;
    break;
  }
  case 21: { // BNNN
    cpu.PC = cpu.V[0] + nnn;
    break;
  }
  case 22: {                         // CXNN
    cpu.V[b2] = nn & (rand() % 255); // TODO: INIT RANDOM !!
    break;
  }
  case 23: { // DXYN
    drawSprite(&cpu, b2, b1, b0);
    break;
  }
  case 24: { // EX9E
    if (strtol(SDL_GetKeyName(event.key.key), NULL, 16) == cpu.V[b2]) {
      cpu.PC += 2;
    }
    break;
  }
  case 25: { // EXA1
    if (strtol(SDL_GetKeyName(event.key.key), NULL, 16) != cpu.V[b2]) {
      cpu.PC += 2;
    }
    break;
  }
  case 26: { // FX07
    cpu.V[b2] = cpu.timerjeu;
    break;
  }
  case 27: { // FX0A
    Uint8 pressed = 0;
    while (!pressed) {
      if ((event.key.down &&
           (event.key.key >= 0x00000061u && event.key.key <= 0x00000066u)) ||
          ((event.key.key >= 0x40000059u && event.key.key <= 0x00000062u))) {
        pressed = 1;
        cpu.V[b2] = strtol(SDL_GetKeyName(event.key.key), NULL, 16);
      }
    }
    break;
  }
  case 28: { // FX15
    cpu.timerjeu = cpu.V[b2];
    break;
  }
  case 29: { // FX18
    cpu.timerson = cpu.V[b2];
    break;
  }
  case 30: { // FX1E
    cpu.i += cpu.V[b2];
    break;
  }
  case 31: { // FX29
    cpu.i = 0xF0 & b2;
    break;
  }
  case 32: { // FX33
    binToBcd(b2, &units, &tens, &hundreds);

    cpu.memory[cpu.i] = units;
    cpu.memory[cpu.i + 1] = tens;
    cpu.memory[cpu.i + 2] = hundreds;
    break;
  }
  case 33: { // FX55
    for (Uint8 i = 0; i < b2; i++) {
      cpu.memory[cpu.i + i] = cpu.V[i];
    }
    break;
  }
  case 34: { // FX65
    for (Uint8 i = 0; i < b2; i++) {
      cpu.V[i] = cpu.memory[cpu.i + i];
    }
    break;
  }
  default: {
  } break;
  }

  cpu.PC += 2;
  printf("PC = %X\n", cpu.PC);
  printf("opCode %X\n", opCode);
}

void decompter() {
  if (cpu.timerson > 0) {
    cpu.timerson--;
  }
  if (cpu.timerjeu > 0) {
    cpu.timerjeu--;
  }
}
Uint16 getOpCode() { return cpu.memory[cpu.PC] << 8 | cpu.memory[cpu.PC + 1]; }
void binToBcd(Uint32 bin, Uint8 *units, Uint8 *tens,
              Uint8 *hundreds) { //

  for (Uint8 i = 0; i <= 7; i++) {
    bin = bin << 1;
    if (((bin >> 8) & 0x0000000F) > 5) {
      bin = ((((bin >> 8) & 0x000F) + 3) << 8) + (bin & 0xFFFFF0FF);
    } // units
    if (((bin >> 12) & 0x0000000F) > 5) {
      bin = ((((bin >> 12) & 0x000F) + 3) << 12) + (bin & 0xFFFF0FFF);
    }
  }
  bin = bin >> 8;
  *units = (bin & 0x000F);
  *tens = (bin & 0x00F0) << 4;
  *hundreds = (0x0F00 & bin) << 8;
}
void loadRom() {
  FILE *rom = fopen("../rom/1-chip8-logo.ch8", "rb");
  if (rom == NULL) {
    printf("File does not exit \n");
    exit(EXIT_FAILURE);
  }
  fread(&cpu.memory[pointerStart], sizeof(Uint8) * (memorySize - pointerStart),
        1, rom);
}

void initFont() { // 5 10 15 20 25
  cpu.memory[0 * FONTHEIGHT] = 0xF0;
  cpu.memory[0 * FONTHEIGHT + 1] = 0x90;
  cpu.memory[0 * FONTHEIGHT + 2] = 0x90;
  cpu.memory[0 * FONTHEIGHT + 3] = 0x90;
  cpu.memory[0 * FONTHEIGHT + 4] = 0xF0; // 0
  cpu.memory[1 * FONTHEIGHT] = 0x20;
  cpu.memory[1 * FONTHEIGHT + 1] = 0x60;
  cpu.memory[1 * FONTHEIGHT + 2] = 0x20;
  cpu.memory[1 * FONTHEIGHT + 3] = 0x20;
  cpu.memory[1 * FONTHEIGHT + 4] = 0x70; // 1
  cpu.memory[2 * FONTHEIGHT] = 0xF0;
  cpu.memory[2 * FONTHEIGHT + 1] = 0x90;
  cpu.memory[2 * FONTHEIGHT + 2] = 0x90;
  cpu.memory[2 * FONTHEIGHT + 3] = 0x90;
  cpu.memory[2 * FONTHEIGHT + 4] = 0xF0; // 2
  cpu.memory[3 * FONTHEIGHT] = 0xF0;
  cpu.memory[3 * FONTHEIGHT + 1] = 0x10;
  cpu.memory[3 * FONTHEIGHT + 2] = 0x70;
  cpu.memory[3 * FONTHEIGHT + 3] = 0x10;
  cpu.memory[3 * FONTHEIGHT + 4] = 0xF0; // 3
  cpu.memory[4 * FONTHEIGHT] = 0x30;
  cpu.memory[4 * FONTHEIGHT + 1] = 0x50;
  cpu.memory[4 * FONTHEIGHT + 2] = 0x90;
  cpu.memory[4 * FONTHEIGHT + 3] = 0xF0;
  cpu.memory[4 * FONTHEIGHT + 4] = 0x10; // 4
  cpu.memory[5 * FONTHEIGHT] = 0xF0;
  cpu.memory[5 * FONTHEIGHT + 1] = 0x10;
  cpu.memory[5 * FONTHEIGHT + 2] = 0xF0;
  cpu.memory[5 * FONTHEIGHT + 3] = 0x80;
  cpu.memory[5 * FONTHEIGHT + 4] = 0xF0; // 5
  cpu.memory[6 * FONTHEIGHT] = 0xF0;
  cpu.memory[6 * FONTHEIGHT + 1] = 0x80;
  cpu.memory[6 * FONTHEIGHT + 2] = 0xF0;
  cpu.memory[6 * FONTHEIGHT + 3] = 0x90;
  cpu.memory[6 * FONTHEIGHT + 4] = 0xF0; // 6
  cpu.memory[7 * FONTHEIGHT] = 0xF0;
  cpu.memory[7 * FONTHEIGHT + 1] = 0x90;
  cpu.memory[7 * FONTHEIGHT + 2] = 0x10;
  cpu.memory[7 * FONTHEIGHT + 3] = 0x10;
  cpu.memory[7 * FONTHEIGHT + 4] = 0x10; // 7
  cpu.memory[8 * FONTHEIGHT] = 0xF0;
  cpu.memory[8 * FONTHEIGHT + 1] = 0x90;
  cpu.memory[8 * FONTHEIGHT + 2] = 0xF0;
  cpu.memory[8 * FONTHEIGHT + 3] = 0x90;
  cpu.memory[8 * FONTHEIGHT + 4] = 0xF0; // 8
  cpu.memory[9 * FONTHEIGHT] = 0xF0;
  cpu.memory[9 * FONTHEIGHT + 1] = 0x90;
  cpu.memory[9 * FONTHEIGHT + 2] = 0xF0;
  cpu.memory[9 * FONTHEIGHT + 3] = 0x10;
  cpu.memory[9 * FONTHEIGHT + 4] = 0xF0; // 9
  cpu.memory[10 * FONTHEIGHT] = 0xF0;
  cpu.memory[10 * FONTHEIGHT + 1] = 0x90;
  cpu.memory[10 * FONTHEIGHT + 2] = 0xF0;
  cpu.memory[10 * FONTHEIGHT + 3] = 0x90;
  cpu.memory[10 * FONTHEIGHT + 4] = 0xF0; // A
  cpu.memory[11 * FONTHEIGHT] = 0xE0;
  cpu.memory[11 * FONTHEIGHT + 1] = 0x90;
  cpu.memory[11 * FONTHEIGHT + 2] = 0xE0;
  cpu.memory[11 * FONTHEIGHT + 3] = 0x90;
  cpu.memory[11 * FONTHEIGHT + 4] = 0xE0; // B
  cpu.memory[12 * FONTHEIGHT] = 0xF0;
  cpu.memory[12 * FONTHEIGHT + 1] = 0x80;
  cpu.memory[12 * FONTHEIGHT + 2] = 0x80;
  cpu.memory[12 * FONTHEIGHT + 3] = 0x80;
  cpu.memory[12 * FONTHEIGHT + 4] = 0xF0; // C
  cpu.memory[13 * FONTHEIGHT] = 0xE0;
  cpu.memory[13 * FONTHEIGHT + 1] = 0x90;
  cpu.memory[13 * FONTHEIGHT + 2] = 0x90;
  cpu.memory[13 * FONTHEIGHT + 3] = 0x90;
  cpu.memory[13 * FONTHEIGHT + 4] = 0xE0; // D
  cpu.memory[14 * FONTHEIGHT] = 0xF0;
  cpu.memory[14 * FONTHEIGHT + 1] = 0x80;
  cpu.memory[14 * FONTHEIGHT + 2] = 0xE0;
  cpu.memory[14 * FONTHEIGHT + 3] = 0x80;
  cpu.memory[14 * FONTHEIGHT + 4] = 0xF0; // E
  cpu.memory[15 * FONTHEIGHT] = 0xF0;
  cpu.memory[15 * FONTHEIGHT + 1] = 0x80;
  cpu.memory[15 * FONTHEIGHT + 2] = 0xE0;
  cpu.memory[15 * FONTHEIGHT + 3] = 0x80;
  cpu.memory[15 * FONTHEIGHT + 4] = 0xC0; // F
}
