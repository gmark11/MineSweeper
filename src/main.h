#ifndef MAIN_H
#define MAIN_H

typedef struct FieldPixelSetting
{
    double field_start_pixel_x;
    double field_start_pixel_y;
    double cell_size;
} FieldPixelSetting;

static void setup_ui(Game *game, Cell **cells);
static void sdl_init(char const *title, int width, int height, SDL_Window **pwindow);
static void detect_menu_click(SDL_Event ev, Game *game, SDL_Texture *background, SDL_Renderer *renderer, bool *menu);
static void menu_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background);
static void game_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background);
static void render_field(SDL_Renderer *renderer, Game *game, Cell **cells, FieldPixelSetting *fpd, SDL_Texture *cell_img);
static void detect_game_click(SDL_Renderer *renderer, SDL_Event ev, Game *game, Cell **cells, FieldPixelSetting *fpd, SDL_Texture *cell_img);

#endif
