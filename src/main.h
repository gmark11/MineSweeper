#ifndef MAIN_H
#define MAIN_H

typedef struct FieldPixelSetting
{
    double field_start_pixel_x;
    double field_start_pixel_y;
    double cell_size;
} FieldPixelSetting;

void setup_ui(Game *game, Cell **cells);
void sdl_init(char const *title, int width, int height, SDL_Window **pwindow);
void detect_menu_click(SDL_Event ev, Game *game, SDL_Texture *background, SDL_Renderer *renderer, bool *menu_on, GameMode *mode, Field *field);
void menu_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background);
void game_view(SDL_Window *window, SDL_Renderer **prenderer, SDL_Texture *background);
void render_field(SDL_Renderer *renderer, Game *game, Cell **cells, FieldPixelSetting *fpd, SDL_Texture *cell_img);
void detect_game_click(SDL_Renderer *renderer, SDL_Event ev, Game *game, Cell **cells, FieldPixelSetting *fpd, SDL_Texture *cell_img);
void destroy_sdl(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *background, SDL_Texture *cell_img, SDL_Texture *result_background, SDL_Surface *clock, SDL_Texture *clock_t);
void result_view(SDL_Renderer *renderer, SDL_Texture *result_background);
void render_clock(SDL_Renderer *renderer, SDL_Surface *clock, SDL_Texture *clock_t);

#endif
