#ifndef CPU_H
#define CPU_H

#include <SDL3/SDL_stdinc.h>
#define memorySize 4096
#define pointerStart 500

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

CPU cpu;

void initCpu();
void decompter();

#endif // !CPU_H
