#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include "ui.h"

void sdl_init(char const *title, int width, int height, SDL_Window **pwindow, SDL_Renderer **prenderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Cant not start SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == NULL)
    {
        SDL_Log("Can not create window: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_Log("Can not create displayer: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

int open_window()
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    /*SDL_Texture* background = IMG_LoadTexture(renderer, "resources/background.png");
    if (background == NULL) {
        SDL_Log("Can not open background image: %s", IMG_GetError());
        exit(1);
    }*/

    sdl_init("MineSweeper", 1280, 720, &window, &renderer);

    //background = SDL_LoadBMP("resources/background.png");
    //background = SDL_CreateTextureFromSurface(renderer, loading_surf);
    //SDL_FreeSurface(loading_surf);

    //SDL_RenderCopy(renderer, background, NULL, NULL);
    SDL_RenderPresent(renderer);
    //SDL_Delay(500);

    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT)
    {
    }

    //SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
