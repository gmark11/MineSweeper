#ifndef MAIN_H
#define MAIN_H

typedef struct FieldPixelSetting
{
    double field_start_pixel_x;
    double field_start_pixel_y;
    double cell_size;
} FieldPixelSetting;

void setup_ui(Game *game, Cell **cells);
void sdl_init(char const *title, int width, int height, SDL_Window **pwindow, SDL_Renderer **prenderer);
void detect_menu_click(SDL_Event ev, Game *game, SDL_Renderer *renderer, bool *menu_on, GameMode *mode, Field *field);
void menu_or_game_view(SDL_Window *window, SDL_Renderer *renderer, int menu_or_game);
void render_field(SDL_Renderer *renderer, Game *game, Cell **cells, FieldPixelSetting *fpd);
void detect_game_click(SDL_Renderer *renderer, SDL_Event ev, Game *game, Cell **cells, FieldPixelSetting *fpd);
void result_view(SDL_Renderer *renderer);
void render_clock(SDL_Renderer *renderer);
void calculate_cells(FieldPixelSetting *fpd, Game *game);

#endif
