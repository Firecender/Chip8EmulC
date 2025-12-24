#include "../Inc/pixel.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <stdlib.h>

void initialiserSDL();
void quitterSDL();
void pause();

int main(int argc, char *argv[]) {
  initScreen();
  initPixel();

  updateScreen();

  pause();

  return EXIT_SUCCESS;
}

void quitterSDL() {

  SDL_DestroyTexture(carre[0]);
  SDL_DestroyTexture(carre[1]);
  SDL_Quit();
}

void pause() {

  Uint8 continuer = 1;

  do {
    SDL_WaitEvent(&event);

    switch (event.type) {
    case SDL_EVENT_QUIT:
      continuer = 0;
      break;
    case SDL_EVENT_KEY_DOWN:
      continuer = 1;
      break;
    default:
      break;
    }
  } while (continuer == 1);
}
