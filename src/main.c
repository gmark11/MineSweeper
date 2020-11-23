#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_ttf.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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
	SDL_Texture *background, *cell_img, *result_background, *clock_t;
	SDL_Surface *clock;
	bool menu_on, game_on, loaded; // These handles different sessions
	bool running = true;
	SDL_Event event;
	GameMode mode = medium_mode; // Default values of MODE
	Field field = medium_field;	 // and FIELD (only used in the menu)

	//WINDOW SETUP
	sdl_init("MineSweeper", 1280, 720, &window);

	//CHECK IF ]
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

	//EVENT CONTROLLER
	while (SDL_WaitEvent(&event))
	{
		//MENU CONTROLLER
		if (menu_on == true)
		{
			// Detect click in menu
			if (event.type == SDL_MOUSEBUTTONDOWN)
				detect_menu_click(event, game, background, renderer, &menu_on, &mode, &field);
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
				{
					cells = setup_cells(game);
					set_time(-1);
				}
				game_view(window, &renderer, background);
				fpd.cell_size = (720 - 2 * 70) / game->field;
				fpd.field_start_pixel_x = (1280 - (720 - 2 * 70)) / 2;
				fpd.field_start_pixel_y = (720 - (720 - 2 * 70)) / 2;
				render_field(renderer, game, cells, &fpd, cell_img);
			}
			else
			{
				// Update timer
				render_clock(renderer, clock, clock_t);
				// If game is won or lost (end screen + start new game)
				if (get_status() != ingame)
				{
					SDL_Delay(2000);
					result_view(renderer, result_background);
					SDL_Delay(3000);
					menu_on = true;
					game_on = false;
					loaded = false;
					destroy_sdl(renderer, window, background, cell_img, result_background, clock, clock_t);
					free_memory(cells, game);
					set_status(ingame);
					menu_view(window, &renderer, background);
				}
				// Detect click during the game
				if (event.type == SDL_MOUSEBUTTONDOWN)
					detect_game_click(renderer, event, game, cells, &fpd, cell_img);
			}
		}
		// QUIT
		if (event.type == SDL_QUIT)
		{
			if (get_status() == ingame && game_on == true)
			{
				save(game, &cells);
				free_memory(cells, game);
			}
			destroy_sdl(renderer, window, background, cell_img, result_background, clock, clock_t);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
		SDL_RenderPresent(renderer);
	}
}

void destroy_sdl(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *background, SDL_Texture *cell_img, SDL_Texture *result_background, SDL_Surface *clock, SDL_Texture *clock_t)
{
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(result_background);
	SDL_DestroyTexture(cell_img);
	SDL_DestroyTexture(clock_t);
	SDL_FreeSurface(clock);
	SDL_DestroyRenderer(renderer);
}

void game_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	*prenderer = renderer;

	background = IMG_LoadTexture(renderer, "resources/game_background.png");

	SDL_Rect background_src = {0, 0, 1920, 1080};
	SDL_Rect background_dest = {0, 0, 1280, 720};
	SDL_RenderCopy(renderer, background, &background_src, &background_dest);
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
}

void menu_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	*prenderer = renderer;

	background = IMG_LoadTexture(renderer, "resources/background.png");

	SDL_Rect src = {0, 0, 1920, 1080};
	SDL_Rect dest = {0, 0, 1280, 720};
	SDL_RenderCopy(renderer, background, &src, &dest);
}

void result_view(SDL_Renderer *renderer, SDL_Texture *result_background)
{
	if (get_status() == win)
		result_background = IMG_LoadTexture(renderer, "resources/win.png");
	if (get_status() == gameover)
		result_background = IMG_LoadTexture(renderer, "resources/lose.png");

	SDL_Rect src = {0, 0, 600, 600};
	SDL_Rect dest = {0, 0, 1280, 720};
	SDL_RenderCopy(renderer, result_background, &src, &dest);
	SDL_RenderPresent(renderer);
}

void render_clock(SDL_Renderer *renderer, SDL_Surface *clock, SDL_Texture *clock_t)
{
	//Get current time info
	update_time();
	double time = get_time();

	SDL_Color white = {255, 255, 255};
	SDL_Color gray = {185, 185, 185};
	SDL_Rect src = {1140, 10, 0, 0};

	TTF_Init();
	TTF_Font *font = TTF_OpenFont("resources/Sans.ttf", 34);

	char min_in_string[3];
	char sec_in_string[3];
	char time_in_string[5];
	sprintf(min_in_string, "%d", (int)time / 60);
	sprintf(sec_in_string, "%d", (int)time % 60);

	//Concatenate strings
	int index = 0;
	if (strlen(min_in_string) == 1)
	{
		time_in_string[index++] = '0';
		time_in_string[index++] = min_in_string[0];
	}
	else
	{
		time_in_string[index++] = min_in_string[0];
		time_in_string[index++] = min_in_string[1];
	}
	time_in_string[index++] = ':';
	if (strlen(sec_in_string) == 1)
	{
		time_in_string[index++] = '0';
		time_in_string[index++] = sec_in_string[0];
	}
	else
	{
		time_in_string[index++] = sec_in_string[0];
		time_in_string[index++] = sec_in_string[1];
	}
	time_in_string[index] = '\0';
	//End

	clock = TTF_RenderText_Shaded(font, time_in_string, white, gray);
	clock_t = SDL_CreateTextureFromSurface(renderer, clock);
	src.w = clock->w;
	src.h = clock->h;
	SDL_RenderCopy(renderer, clock_t, NULL, &src);
	TTF_CloseFont(font);
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
	//If the game field is clicked
	if (ev.motion.x >= fpd->field_start_pixel_x && ev.motion.x <= (1280 - fpd->field_start_pixel_x) && ev.motion.y >= fpd->field_start_pixel_y && ev.motion.y <= (720 - fpd->field_start_pixel_y))
	{
		int x = (ev.motion.x - (int)fpd->field_start_pixel_x) / (int)fpd->cell_size;
		int y = (ev.motion.y - (int)fpd->field_start_pixel_y) / (int)fpd->cell_size;
		if (ev.button.button == SDL_BUTTON_LEFT)
			show(game, &cells, x, y);
		if (ev.button.button == SDL_BUTTON_RIGHT)
			mark(&cells, x, y);

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
