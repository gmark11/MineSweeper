/// @file main.c
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"

static void setup_ui(Game *game, Cell **cells);
static void sdl_init(char const *title, int width, int height, SDL_Window **pwindow);
static void detect_menu_click(SDL_Event ev, Game *game, SDL_Texture *background, SDL_Renderer *renderer, bool *menu);
static void menu_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background);
static void game_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background);
static void render_field(SDL_Renderer *renderer, Game *game, Cell **cells, double cell_size, double field_start_pixel_x, double field_start_pixel_y, SDL_Texture *cell_img);
static void detect_game_click(SDL_Renderer *renderer, SDL_Event ev, Game *game, Cell **cells, double field_start_pixel_x, double field_start_pixel_y, double cell_size, SDL_Texture *cell_img);

/**
 * Main function
 * @param argc Number of parameters
 * @param argv Pointer of the array of marameters
 * @return 0 if there is not any error
 */
int main(int argc, char *argv[])
{
	Game game;
	Cell **cells;
	setup_ui(&game, cells);
	return 0;
}

/**
 * This function handles the whole SDL UI
 * @param game Pointer of the game
 * @param cells Pointer of the cells
 */
static void setup_ui(Game *game, Cell **cells)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *background, *cell_img;
	bool menu_on;
	bool game_on;

	//WINDOW SETUP
	sdl_init("MineSweeper", 1280, 720, &window);

	//INIT START MENU
	menu_on = true;
	game_on = false;
	menu_view(window, &renderer, background);

	//EVENT CONTROLLER
	SDL_Event ev;
	while (SDL_WaitEvent(&ev))
	{
		//MENU CONTROLLER
		if (menu_on == true)
		{
			if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				detect_menu_click(ev, game, background, renderer, &menu_on);
			}
		}
		//GAME CONTROLLER
		if (menu_on == false)
		{
			double cell_size;
			double field_start_pixel_x;
			double field_start_pixel_y;
			if (game_on == false)
			{
				//INIT GAME
				game_on = true;
				game_view(window, &renderer, background);
				cells = setup_cells(game);
				cell_size = (720 - 2 * 70) / game->field;
				field_start_pixel_x = (1280 - (720 - 2 * 70)) / 2;
				field_start_pixel_y = (720 - (720 - 2 * 70)) / 2;
				render_field(renderer, game, cells, cell_size, field_start_pixel_x, field_start_pixel_y, cell_img);
			}
			else
			{
				if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					detect_game_click(renderer, ev, game, cells, field_start_pixel_x, field_start_pixel_y, cell_size, cell_img);
				}
			}
		}
		//QUIT
		if (ev.type == SDL_QUIT)
		{
			for (int y = 0; y < game->field; y++)
			{
				free(cells[y]);
			}
			free(cells);
			SDL_DestroyTexture(background);
			SDL_DestroyTexture(cell_img);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	}
}

/**
 * This function renders the background of the game view
 * @param window Pointer of the SDL window
 * @param prenderer Pointer of the SDL renderer
 * @param background Pointer of the background
 */
static void game_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background)
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

/**
 * This function renders the background of the game view
 * @param window Pointer of the SDL window
 * @param prenderer Pointer of the SDL renderer
 * @param background Pointer of the background image
 */
static void render_field(SDL_Renderer *renderer, Game *game, Cell **cells, double cell_size, double field_start_pixel_x, double field_start_pixel_y, SDL_Texture *cell_img)
{
	for (int x = field_start_pixel_x; x < game->field * cell_size + field_start_pixel_x; x = x + cell_size)
	{
		for (int y = field_start_pixel_y; y < game->field * cell_size + field_start_pixel_y; y = y + cell_size)
		{
			int index_x = (x - field_start_pixel_x) / cell_size;
			int index_y = (y - field_start_pixel_y) / cell_size;
			if (cells[index_x][index_y].shown == true)
			{
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
				case four:
					cell_img = IMG_LoadTexture(renderer, "resources/4.png");
					break;
				case five:
					cell_img = IMG_LoadTexture(renderer, "resources/5.png");
					break;
				}
			}
			else if (cells[index_x][index_y].marked == true)
			{
				cell_img = IMG_LoadTexture(renderer, "resources/marked.png");
			}
			else
			{
				cell_img = IMG_LoadTexture(renderer, "resources/facingDown.png");
			}
			SDL_Rect fd_cell_src = {0, 0, 200, 200};
			SDL_Rect fd_cell_dest = {x, y, cell_size, cell_size};
			SDL_RenderCopy(renderer, cell_img, &fd_cell_src, &fd_cell_dest);
		}
	}
	SDL_RenderPresent(renderer);
}

/**
 * This function renders the background of the menu view
 * @param window Pointer of the SDL window
 * @param prenderer Pointer of the SDL renderer
 * @param background Pointer of the background image
 */
static void menu_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL)
	{
		SDL_Log("Can not create renderer: %s", SDL_GetError());
		exit(1);
	}
	*prenderer = renderer;
	background = IMG_LoadTexture(renderer, "resources/background.png");
	if (background == NULL)
	{
		SDL_Log("Can not open background image: %s", IMG_GetError());
		exit(1);
	}

	SDL_Rect src = {0, 0, 1920, 1080};
	SDL_Rect dest = {0, 0, 1280, 720};
	SDL_RenderCopy(renderer, background, &src, &dest);
	SDL_RenderPresent(renderer);
}

/**
 * Detects clicks in the menu
 * @param ev SDL event
 * @param game Pointer of the game
 * @param background Pointer of the background image
 * @param renderer Pointer of the SDL renderer
 * @param menu_on Pointer of a boolean which handles if the game is in the menu or not
 */
static void detect_menu_click(SDL_Event ev, Game *game, SDL_Texture *background, SDL_Renderer *renderer, bool *menu_on)
{
	GameMode mode = easy_mode;
	Field field = small_field; //TODO: DO not set to default because of loop
							   //FIELD SIZE
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
	//START BUTTON
	if (ev.motion.x >= 485 && ev.motion.x <= 800 && ev.motion.y >= 460 && ev.motion.y <= 560)
	{
		SDL_DestroyTexture(background);
		SDL_DestroyRenderer(renderer);
		*menu_on = false;
		new_game(game, mode, field);
	}
}

/**
 * Detects clicks during the game
 * @param renderer Pointer of the SDL renderer
 * @param ev SDL event
 * @param game Pointer of the game
 * @param cells Pointer of the cells
 * @param field_start_pixel_x Starting pixel on the x-coordinate
 * @param field_start_pixel_y Starting pixel on the y-coordinate
 * @param cell_size The length and width of a cell in pixels
 * @param cell_img Pointer of the image of the cell
 */
static void detect_game_click(SDL_Renderer *renderer, SDL_Event ev, Game *game, Cell **cells, double field_start_pixel_x, double field_start_pixel_y, double cell_size, SDL_Texture *cell_img)
{
	if (ev.motion.x >= field_start_pixel_x && ev.motion.x <= (1280 - field_start_pixel_x) && ev.motion.y >= field_start_pixel_y && ev.motion.y <= (720 - field_start_pixel_y))
	{
		if (ev.button.button == SDL_BUTTON_LEFT)
		{
			int x = (ev.motion.x - (int)field_start_pixel_x) / (int)cell_size;
			int y = (ev.motion.y - (int)field_start_pixel_y) / (int)cell_size;
			show(&cells, x, y);
		}
		else if (ev.button.button == SDL_BUTTON_RIGHT)
		{
			int x = (ev.motion.x - (int)field_start_pixel_x) / (int)cell_size;
			int y = (ev.motion.y - (int)field_start_pixel_y) / (int)cell_size;
			mark(&cells, x, y);
		}
		render_field(renderer, game, cells, cell_size, field_start_pixel_x, field_start_pixel_y, cell_img);
	}
}

/**
 * Creates the SDL window
 * @param title Name of the window
 * @param width Width of the window
 * @param height Height of the window
 * @param pwindow Pointer of the window
 */
static void sdl_init(char const *title, int width, int height, SDL_Window **pwindow)
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
	*pwindow = window;
}
