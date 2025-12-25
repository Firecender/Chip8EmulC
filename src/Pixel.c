#include "../Inc/pixel.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

PIXEL pixels[LONGUEUR][LARGEUR];
SDL_Renderer *rend;
SDL_Event event;
SDL_Texture *carre[2];
SDL_Texture *smallTest;

void initPixel() {
  for (int i = 0; i < LONGUEUR; i++) {
    for (int j = 0; j < LARGEUR; j++) {
      pixels[i][j].color = NOIR;

      pixels[i][j].pos.x = i * pixelPerpixel;
      pixels[i][j].pos.y = j * pixelPerpixel;
      pixels[i][j].pos.w = pixelPerpixel;
      pixels[i][j].pos.h = pixelPerpixel;
    }
  }
}

void initScreen() {

  SDL_Window *win;
  SDL_Surface *temp;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    printf("ff : %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_CreateWindowAndRenderer("Chip8", SCREENHEIGHT, SCREENWIDHT,
                              SDL_WINDOW_MAXIMIZED, &win, &rend);
  SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  temp =
      SDL_CreateSurface(pixelPerpixel, pixelPerpixel, SDL_PIXELFORMAT_XRGB8888);
  if (temp == NULL) {
    printf("Oh shit : %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (!SDL_FillSurfaceRect(
          temp, NULL,
          SDL_MapRGB(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_XRGB8888), NULL,
                     0, 0, 0))) {
    printf("Erreur fill noir : %s \n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  carre[0] = SDL_CreateTextureFromSurface(rend, temp);
  SDL_DestroySurface(temp);

  temp =
      SDL_CreateSurface(pixelPerpixel, pixelPerpixel, SDL_PIXELFORMAT_XRGB8888);
  if (temp == NULL) {
    printf("Erreur création de surface carre1 : %s \n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  if (!SDL_FillSurfaceRect(
          temp, NULL,
          SDL_MapRGB(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_XRGB8888), NULL,
                     255, 255, 255))) {
    printf("Erreur fill blanc : %s \n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  carre[1] = SDL_CreateTextureFromSurface(rend, temp);
  SDL_DestroySurface(temp);
}

void showPixelsState(PIXEL p) {
  SDL_FRect toSend;
  toSend.h = 0;
  toSend.x = 0;
  toSend.y = 0;
  toSend.w = 0;
  SDL_RectToFRect(&p.pos, &toSend);
  SDL_RenderTexture(rend, carre[p.color], NULL, &toSend);
}

void clearScreen() {
  for (int i = 0; i < LONGUEUR; i++) {
    for (int j = 0; i < LARGEUR; j++) {
      pixels[i][j].color = NOIR;
    }
  }
}
void updateScreen() {
  SDL_RenderClear(rend);
  for (int i = 0; i < LONGUEUR; i++) {
    for (int j = 0; j < LARGEUR; j++) {
      showPixelsState(pixels[i][j]);
    }
  }

  SDL_RenderPresent(rend);
}
void drawSprite(CPU *cpu, Uint8 b2, Uint8 b1, Uint8 b0) {
  Uint8 toShow = 0;
  Uint8 x = cpu->V[b2];
  Uint8 y = cpu->V[b1];

  for (Uint8 k = 0; k < cpu->V[b0]; k++) { // 1101 0101
    if (k + y > LARGEUR) {
      break;
    }
    toShow = cpu->memory[cpu->i + k];
    for (Uint8 d = 0; d < 8; d++) { //
      if (d + cpu->V[b2] > LONGUEUR) {
        break;
      }
      if ((toShow >> d) & 0x01) {
        if (pixels[x + (7 - d)][y + k].color == 1) {
          cpu->V[0xF] = 1;
          pixels[x + (7 - d)][y + k].color = 0;
        } else {
          pixels[x + (7 - d)][y + k].color = 1;
        }
      }
    }
  }
}
