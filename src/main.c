#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"


static int ui_setup(Game *game);
static void sdl_init(char
	const *title, int width, int height, SDL_Window **pwindow, SDL_Renderer **prenderer);
static void detect_click_on_start(SDL_Event ev, Game *game);

int main(int argc, char *argv[])
{
	Game game;
	//Cell**cells = setup(game, 2);
	ui_setup(&game);
	printf("%d %d %d", game.field, game.mode, game.x);

	return 0;
}

static int ui_setup(Game *game)
{
	SDL_Window * window;
	SDL_Renderer * renderer;
	sdl_init("MineSweeper", 1280, 720, &window, &renderer);

	//background
	SDL_Texture *background = IMG_LoadTexture(renderer, "resources/background.png");
	if (background == NULL)
	{
		SDL_Log("Can not open background image: %s", IMG_GetError());
		exit(1);
	}

	SDL_Rect src = { 0,
		0,
		1920,
		1080
	};
	SDL_Rect dest = { 0,
		0,
		1280,
		720
	};
	SDL_RenderCopy(renderer, background, &src, &dest);
	SDL_RenderPresent(renderer);
	//end_background

	SDL_Event ev;
	while (SDL_WaitEvent(&ev))
	{
		if (ev.type == SDL_MOUSEBUTTONDOWN)
		{
			detect_click_on_start(ev, game);
		}

		if (ev.type == SDL_QUIT)
		{
			SDL_DestroyTexture(background);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	}

	return 0;
}

static void detect_click_on_start(SDL_Event ev, Game *game)
{
	GameMode mode = small_field;
	Field field = small_field;

	if (ev.motion.x >= 160 && ev.motion.x <= 470 && ev.motion.y >= 235 && ev.motion.y <= 300)
	{
		field = small_field;
	}

	if (ev.motion.x >= 485 && ev.motion.x <= 795 && ev.motion.y >= 235 && ev.motion.y <= 300)
	{
		field = medium_field;
	}

	if (ev.motion.x >= 810 && ev.motion.x <= 1120 && ev.motion.y >= 235 && ev.motion.y <= 300)
	{
		field = big_field;
	}

	//GAME MODE
	if (ev.motion.x >= 160 && ev.motion.x <= 470 && ev.motion.y >= 325 && ev.motion.y <= 390)
	{
		mode = easy_mode;
	}

	if (ev.motion.x >= 485 && ev.motion.x <= 795 && ev.motion.y >= 325 && ev.motion.y <= 390)
	{
		mode = medium_mode;
	}

	if (ev.motion.x >= 810 && ev.motion.x <= 1120 && ev.motion.y >= 325 && ev.motion.y <= 390)
	{
		mode = hard_mode;
	}

	//START
	if (ev.motion.x >= 485 && ev.motion.x <= 800 && ev.motion.y >= 460 && ev.motion.y <= 560)
	{
	    printf("%d %d\n", field, mode);
		new_game(mode, field);
	}
}

static void sdl_init(char
	const *title, int width, int height, SDL_Window **pwindow, SDL_Renderer **prenderer)
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
