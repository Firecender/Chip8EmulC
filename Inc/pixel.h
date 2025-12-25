#ifndef PIXEL_H
#define PIXEL_H
#include "cpu.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>

#define NOIR 0
#define BLANC 1

#define LONGUEUR 64
#define LARGEUR 32

#define pixelPerpixel 8

#define SCREENWIDHT LARGEUR *pixelPerpixel
#define SCREENHEIGHT LONGUEUR *pixelPerpixel

typedef struct {
  SDL_Rect pos;
  Uint8 color;
} PIXEL;

extern SDL_Event event;
extern SDL_Texture *carre[2];

void initPixel();
void initScreen();
void showPixelsState(PIXEL p);
void clearScreen();
void updateScreen();
void drawSprite(CPU *cpu, Uint8 b2, Uint8 b1, Uint8 b0);

#endif // !PIXEL_H
