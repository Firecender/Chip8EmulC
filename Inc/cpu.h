#ifndef CPU_H
#define CPU_H

#include <SDL3/SDL_stdinc.h>
#define memorySize 4096
#define pointerStart 500
#define NBOPCODE 35
#define FONTHEIGHT 5

typedef struct {
  Uint8 memory[memorySize]; // mémoire Chip8 Uint8
  Uint16 PC;

  // timer
  Uint8 timerjeu;
  Uint8 timerson;

  // stack
  Uint8 stackPointeur;
  Uint8 stack[16];

  // Registre chip8
  Uint8 V[16];

  // special register
  Uint16 i;
} CPU;

typedef struct {
  Uint16 mask[NBOPCODE];
  Uint16 code[NBOPCODE];
} opTable;

void initCpu();
void initopTable();
void decompter();
Uint16 getOpCode();
void executeOp(Uint16 opCode);
void stripOpCode(Uint16 opCode, Uint8 *b0, Uint8 *b1, Uint8 *b2, Uint8 *b3);
void binToBcd(Uint32 bin, Uint8 *units, Uint8 *tens, Uint8 *hundreds);
void initFont();
void loadRom();
#endif // !CPU_H
