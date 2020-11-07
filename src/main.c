#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"


static int setup(Game *game, Cell** cells);
static void sdl_init(char const *title, int width, int height, SDL_Window * window);
static void detect_menu_click(SDL_Event ev, Game *game, SDL_Texture *background, SDL_Renderer * renderer);
static void menu(SDL_Window * window, SDL_Renderer * renderer, SDL_Texture *background);
static void game(SDL_Window * window, SDL_Renderer * renderer, SDL_Texture *background);

int main(int argc, char *argv[])
{
	Game game;
	Cell **cells;
	setup(&game, cells);
	return 0;
}

void static int setup(Game *game, Cell **cells)
{
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Texture *menu_background;

	//WINDOW SETUP
	sdl_init("MineSweeper", 1280, 720, &window);

	//START MENU
	menu(window, renderer, menu_background);
	
	//GAME VIEW
	game();

	//EVENT CONTROLLER
	SDL_Event ev;
	while (SDL_WaitEvent(&ev))
	{
		if (menu_background!=NULL && ev.type == SDL_MOUSEBUTTONDOWN)
		{
			detect_menu_click(ev, game, menu_background, renderer);
		}

		if (ev.type == SDL_QUIT)
		{
			if(menu_background!=NULL){
				SDL_DestroyTexture(menu_background);
			}
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	}

	return 0;
}

static void game(SDL_Window * window, SDL_Renderer * renderer, SDL_Texture *background){
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL)
	{
		SDL_Log("Can not create renderer: %s", SDL_GetError());
		exit(1);
	}
	background = IMG_LoadTexture(renderer, "resources/background.png");
	if (background == NULL)
	{
		SDL_Log("Can not open background image: %s", IMG_GetError());
		exit(1);
	}

	SDL_Rect src = { 0, 0, 1920, 1080 };
	SDL_Rect dest = { 0, 0, 1280, 720 };
	SDL_RenderCopy(renderer, background, &src, &dest);
	SDL_RenderPresent(renderer);
}

static void menu(SDL_Window * window, SDL_Renderer * renderer, SDL_Texture *background){
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL)
	{
		SDL_Log("Can not create renderer: %s", SDL_GetError());
		exit(1);
	}
	background = IMG_LoadTexture(renderer, "resources/background.png");
	if (background == NULL)
	{
		SDL_Log("Can not open background image: %s", IMG_GetError());
		exit(1);
	}

	SDL_Rect src = { 0, 0, 1920, 1080 };
	SDL_Rect dest = { 0, 0, 1280, 720 };
	SDL_RenderCopy(renderer, background, &src, &dest);
	SDL_RenderPresent(renderer);

}

static void detect_menu_click(SDL_Event ev, Game *game, SDL_Texture *background, SDL_Renderer * renderer)
{
	GameMode mode = small_field;
	Field field = small_field;

	if (ev.motion.x >= 160 && ev.motion.x <= 470 && ev.motion.y >= 235 && ev.motion.y <= 300){
		field = small_field;
	}

	if (ev.motion.x >= 485 && ev.motion.x <= 795 && ev.motion.y >= 235 && ev.motion.y <= 300){
		field = medium_field;
	}

	if (ev.motion.x >= 810 && ev.motion.x <= 1120 && ev.motion.y >= 235 && ev.motion.y <= 300){
		field = big_field;
	}

	//GAME MODE
	if (ev.motion.x >= 160 && ev.motion.x <= 470 && ev.motion.y >= 325 && ev.motion.y <= 390){
		mode = easy_mode;
	}

	if (ev.motion.x >= 485 && ev.motion.x <= 795 && ev.motion.y >= 325 && ev.motion.y <= 390){
		mode = medium_mode;
	}

	if (ev.motion.x >= 810 && ev.motion.x <= 1120 && ev.motion.y >= 325 && ev.motion.y <= 390){
		mode = hard_mode;
	}
	//START BUTTON
	if (ev.motion.x >= 485 && ev.motion.x <= 800 && ev.motion.y >= 460 && ev.motion.y <= 560){
	    printf("%d %d\n", field, mode);
		new_game(mode, field);
		SDL_DestroyTexture(background);
		SDL_DestroyRenderer(renderer);
		//SDL_RenderClear(renderer);
	}
}

static void sdl_init(char const *title, int width, int height, SDL_Window * window)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("Cant not start SDL: %s", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (window == NULL)
	{
		SDL_Log("Can not create window: %s", SDL_GetError());
		exit(1);
	}
}
