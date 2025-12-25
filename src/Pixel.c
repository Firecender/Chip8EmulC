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

void drawPixel(PIXEL p) {
  SDL_FRect *toSend = malloc(sizeof(SDL_FRect));
  SDL_RectToFRect(&p.pos, toSend);
  SDL_RenderTexture(rend, carre[p.color], NULL, toSend);
  free(toSend);
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
      drawPixel(pixels[i][j]);
    }
  }

  SDL_RenderPresent(rend);
}
