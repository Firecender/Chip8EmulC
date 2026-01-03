#include "../Inc/pixel.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <stdint.h>
#include <stdlib.h>
#define FPS 16 // millisec
#define ops 4  // op per cycle
#include "../Inc/cpu.h"

void initialiserSDL();
void quitterSDL();
void pause();
void mainLoop();
void step();
void standBy();

int main(int argc, char *argv[]) {
  initScreen();
  initPixel();

  updateScreen();
  initCpu();
  loadRom();
  standBy();

  return EXIT_SUCCESS;
}

void quitterSDL() {

  SDL_DestroyTexture(carre[0]);
  SDL_DestroyTexture(carre[1]);
  SDL_Quit();
}
void mainLoop() {
  Uint8 active = 1;
  while (active) {
    for (Uint8 compteur = 0; compteur < ops; compteur++) {
      executeOp(getOpCode());
    }
    updateScreen();
    SDL_Delay(FPS);
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_KEY_DOWN) {
      if (event.key.key == SDLK_ESCAPE) {
        active = 0;
      }
    }
  }
}
void step() {
  for (Uint8 compteur = 0; compteur < ops; compteur++) {
    executeOp(getOpCode());
  }
  updateScreen();
}
void standBy() {
  while (1) {
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_KEY_DOWN) {
      if (event.key.key == SDLK_ESCAPE) {
        break;
      }
      if (event.key.key == SDLK_S) {
        step();
      }
      if (event.key.key == SDLK_C) {
        mainLoop();
      }
    }
    SDL_Delay(200);
  }
}
