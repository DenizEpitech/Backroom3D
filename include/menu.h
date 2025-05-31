/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu
*/

#ifndef MENU_H_
    #define MENU_H_
    #include <SFML/Graphics.h>
    #include <SFML/Audio.h>
    #include "fps.h"

typedef enum {
    NORMAL,
    HOVER,
    CLICKED
} button_state_t;

typedef struct button_s {
    sfRectangleShape *rect;
    sfTexture *texture;
    sfVector2f position;
    sfVector2f size;
    button_state_t state;
} button_t;

typedef struct menu_s {
    sfTexture *background_texture;
    sfSprite *background;
    button_t *start_button;
    button_t *settings_button;
    button_t *exit_button;
    sfMusic *music;
} menu_t;

typedef enum {
    MENU,
    GAME,
    SETTINGS,
    EXIT
} game_state_t;

typedef struct game_s {
    sfRenderWindow *window;
    sfEvent event;
    game_state_t state;
    menu_t *menu;
    fps_info_t *fps_info;
    sfClock *test_clock;
    sfText *clock_text;
    sfFont *font;
    char *map_path;
} game_t;

typedef struct level_t {
    sfRenderWindow *window;
    game_state_t *state;
    fps_info_t *fps_info;
    struct game_context_s *ctx;
    const char *map_path;
    struct player_s *player;
} level_t;

typedef struct background_s {
    sfTexture *texture;
    sfSprite *sprite;
} background_t;

button_t *create_button(const char *texture_path,
    sfVector2f position, sfVector2f size);
void destroy_button(button_t *button);
void update_button_state(button_t *button,
    sfVector2i mouse_pos, sfBool clicked);
void draw_button(sfRenderWindow *window, button_t *button);

menu_t *create_menu(void);
void destroy_menu(menu_t *menu);
void display_menu(sfRenderWindow *window, menu_t *menu);
void handle_menu_events(sfRenderWindow *window, sfEvent event,
    menu_t *menu, game_state_t *state);
void free_menu_resources(menu_t *menu);

#endif
