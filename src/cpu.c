#include "../Inc/cpu.h"

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
void decompter() {
  if (cpu.timerson > 0) {
    cpu.timerson--;
  }
  if (cpu.timerjeu > 0) {
    cpu.timerjeu--;
  }
}
