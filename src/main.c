#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "debugmalloc.h"
#include "game.h"
#include "main.h"

int main(int argc, char *argv[])
{
	Game game;
	Cell **cells;
	setup_ui(&game, cells);
	return 0;
}

void setup_ui(Game *game, Cell **cells)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *background, *cell_img, *result_background;
	bool menu_on, game_on, loaded;

	//WINDOW SETUP
	sdl_init("MineSweeper", 1280, 720, &window);

	//CHECK IF SAVED
	if (load(game, &cells) == true)
	{
		menu_on = false;
		game_on = false;
		loaded = true;
	}
	else
	{
		menu_on = true;
		game_on = false;
		loaded = false;
		menu_view(window, &renderer, background);
	}

	//Default INIT of MODE and FIELD (only used in the menu)
	GameMode mode = medium_mode;
	Field field = medium_field;
	//EVENT CONTROLLER
	SDL_Event ev;
	while (SDL_WaitEvent(&ev))
	{
		//MENU CONTROLLER
		if (menu_on == true)
		{
			if (ev.type == SDL_MOUSEBUTTONDOWN)
				detect_menu_click(ev, game, background, renderer, &menu_on, &mode, &field);
		}
		//GAME CONTROLLER
		if (menu_on == false)
		{
			FieldPixelSetting fpd;
			if (game_on == false)
			{
				//INIT GAME
				game_on = true;
				if (loaded == false)
					cells = setup_cells(game);
				game_view(window, &renderer, background);
				fpd.cell_size = (720 - 2 * 70) / game->field;
				fpd.field_start_pixel_x = (1280 - (720 - 2 * 70)) / 2;
				fpd.field_start_pixel_y = (720 - (720 - 2 * 70)) / 2;
				render_field(renderer, game, cells, &fpd, cell_img);
			}
			else
			{
				if (get_status() != ingame)
				{
					sleep(2);
					result_view(renderer, result_background);
					sleep(3); //TODO: not 100%, this is the best method
					menu_on = true;
					game_on = false;
					loaded = false;
					destroy_sdl(renderer, window, background, cell_img, result_background);
					free_memory(cells, game);
					set_status(ingame);
					menu_view(window, &renderer, background);
				}
				if (ev.type == SDL_MOUSEBUTTONDOWN)
					detect_game_click(renderer, ev, game, cells, &fpd, cell_img);
			}
		}
		//QUIT
		if (ev.type == SDL_QUIT)
		{
			if (get_status() == ingame && game_on == true)
			{
				save(game, &cells);
				free_memory(cells, game);
			}
			destroy_sdl(renderer, window, background, cell_img, result_background);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	}
}

void destroy_sdl(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *background, SDL_Texture *cell_img, SDL_Texture *result_background)
{
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(result_background);
	SDL_DestroyTexture(cell_img);
	SDL_DestroyRenderer(renderer);
}

void free_memory(Cell **cells, Game *game)
{
	for (int y = 0; y < game->field; y++)
		free(cells[y]);
	free(cells);
}

void game_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL)
	{
		SDL_Log("Can not create renderer: %s", SDL_GetError());
		exit(1);
	}
	*prenderer = renderer;

	background = IMG_LoadTexture(renderer, "resources/game_background.png");
	if (background == NULL)
	{
		SDL_Log("Can not open background image: %s", IMG_GetError());
		exit(1);
	}

	SDL_Rect background_src = {0, 0, 1920, 1080};
	SDL_Rect background_dest = {0, 0, 1280, 720};
	SDL_RenderCopy(renderer, background, &background_src, &background_dest);
	SDL_RenderPresent(renderer);
}

void render_field(SDL_Renderer *renderer, Game *game, Cell **cells, FieldPixelSetting *fpd, SDL_Texture *cell_img)
{
	int covered_cells = game->field * game->field;
	for (int x = fpd->field_start_pixel_x; x < game->field * fpd->cell_size + fpd->field_start_pixel_x; x = x + fpd->cell_size)
	{
		for (int y = fpd->field_start_pixel_y; y < game->field * fpd->cell_size + fpd->field_start_pixel_y; y = y + fpd->cell_size)
		{
			int index_x = (x - fpd->field_start_pixel_x) / fpd->cell_size;
			int index_y = (y - fpd->field_start_pixel_y) / fpd->cell_size;
			if (cells[index_x][index_y].shown == true)
			{
				covered_cells--;
				switch (cells[index_x][index_y].type)
				{
				case simple:
					cell_img = IMG_LoadTexture(renderer, "resources/0.png");
					break;
				case one:
					cell_img = IMG_LoadTexture(renderer, "resources/1.png");
					break;
				case two:
					cell_img = IMG_LoadTexture(renderer, "resources/2.png");
					break;
				case three:
					cell_img = IMG_LoadTexture(renderer, "resources/3.png");
					break;
				case bomb:
					cell_img = IMG_LoadTexture(renderer, "resources/bomb.png");
					break;
				}
			}
			else if (cells[index_x][index_y].marked == true)
				cell_img = IMG_LoadTexture(renderer, "resources/marked.png");
			else
				cell_img = IMG_LoadTexture(renderer, "resources/facingDown.png");
			SDL_Rect fd_cell_src = {0, 0, 200, 200};
			SDL_Rect fd_cell_dest = {x, y, fpd->cell_size, fpd->cell_size};
			SDL_RenderCopy(renderer, cell_img, &fd_cell_src, &fd_cell_dest);
		}
	}
	check_win(game, &covered_cells);
	SDL_RenderPresent(renderer);
}

void menu_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	*prenderer = renderer;

	background = IMG_LoadTexture(renderer, "resources/background.png");

	SDL_Rect src = {0, 0, 1920, 1080};
	SDL_Rect dest = {0, 0, 1280, 720};
	SDL_RenderCopy(renderer, background, &src, &dest);
	SDL_RenderPresent(renderer);
}

void result_view(SDL_Renderer *renderer, SDL_Texture *result_background)
{
	if (get_status() == win)
	{
		result_background = IMG_LoadTexture(renderer, "resources/win.png");
	}
	if (get_status() == gameover)
	{
		result_background = IMG_LoadTexture(renderer, "resources/lose.png");
	}

	SDL_Rect src = {0, 0, 600, 600};
	SDL_Rect dest = {0, 0, 1280, 720};
	SDL_RenderCopy(renderer, result_background, &src, &dest);
	SDL_RenderPresent(renderer);
}

void detect_menu_click(SDL_Event ev, Game *game, SDL_Texture *background, SDL_Renderer *renderer, bool *menu_on, GameMode *mode, Field *field)
{
	if (ev.motion.x >= 160 && ev.motion.x <= 470 && ev.motion.y >= 235 && ev.motion.y <= 300)
		*field = small_field;

	if (ev.motion.x >= 485 && ev.motion.x <= 795 && ev.motion.y >= 235 && ev.motion.y <= 300)
		*field = medium_field;

	if (ev.motion.x >= 810 && ev.motion.x <= 1120 && ev.motion.y >= 235 && ev.motion.y <= 300)
		*field = big_field;

	//GAME MODE
	if (ev.motion.x >= 160 && ev.motion.x <= 470 && ev.motion.y >= 325 && ev.motion.y <= 390)
		*mode = easy_mode;

	if (ev.motion.x >= 485 && ev.motion.x <= 795 && ev.motion.y >= 325 && ev.motion.y <= 390)
		*mode = medium_mode;

	if (ev.motion.x >= 810 && ev.motion.x <= 1120 && ev.motion.y >= 325 && ev.motion.y <= 390)
		*mode = hard_mode;
	//START BUTTON
	if (ev.motion.x >= 485 && ev.motion.x <= 800 && ev.motion.y >= 460 && ev.motion.y <= 560)
	{
		SDL_DestroyTexture(background);
		SDL_DestroyRenderer(renderer);
		*menu_on = false;
		new_game(game, *mode, *field);
	}
}

void detect_game_click(SDL_Renderer *renderer, SDL_Event ev, Game *game, Cell **cells, FieldPixelSetting *fpd, SDL_Texture *cell_img)
{
	if (ev.motion.x >= fpd->field_start_pixel_x && ev.motion.x <= (1280 - fpd->field_start_pixel_x) && ev.motion.y >= fpd->field_start_pixel_y && ev.motion.y <= (720 - fpd->field_start_pixel_y))
	{
		if (ev.button.button == SDL_BUTTON_LEFT)
		{
			int x = (ev.motion.x - (int)fpd->field_start_pixel_x) / (int)fpd->cell_size;
			int y = (ev.motion.y - (int)fpd->field_start_pixel_y) / (int)fpd->cell_size;
			show(game, &cells, x, y);
		}
		else if (ev.button.button == SDL_BUTTON_RIGHT)
		{
			int x = (ev.motion.x - (int)fpd->field_start_pixel_x) / (int)fpd->cell_size;
			int y = (ev.motion.y - (int)fpd->field_start_pixel_y) / (int)fpd->cell_size;
			mark(&cells, x, y);
		}
		render_field(renderer, game, cells, fpd, cell_img);
	}
}

void sdl_init(char const *title, int width, int height, SDL_Window **pwindow)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("Can not start SDL: %s", SDL_GetError());
		exit(1);
	}

	SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (window == NULL)
	{
		SDL_Log("Can not create window: %s", SDL_GetError());
		exit(1);
	}
	*pwindow = window;
}
