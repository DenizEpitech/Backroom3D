/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** setting
*/

#ifndef SETTING_H_
    #define SETTING_H_
    #include <SFML/Graphics.h>
    #include <SFML/Audio.h>
    #include "menu.h"

typedef struct volume_s {
    sfRectangleShape *bar;
    sfCircleShape *knob;
    float level;
    sfVector2f bar_pos;
    float bar_width;
    sfBool dragging;
} volume_t;

typedef struct settings_s {
    background_t background;
    button_t *resolution1_button;
    button_t *resolution2_button;
    volume_t volume;
    sfMusic *music;
} settings_t;

typedef struct settings_event_context_s {
    sfRenderWindow **window;
    sfEvent event;
    settings_t *settings;
    sfVector2i mouse_pos;
    sfBool *is_open;
    game_state_t *state;
    sfBool *resolution_changed;
} settings_event_context_t;

typedef struct mouse_button_context_s {
    sfRenderWindow **window;
    sfEvent event;
    settings_t *settings;
    sfVector2i mouse_pos;
    sfBool *is_open;
    game_state_t *state;
    sfBool *resolution_changed;
} mouse_button_context_t;

typedef struct settings_interface_context_s {
    sfRenderWindow **window;
    sfEvent event;
    settings_t *settings;
    sfBool *is_open;
    game_state_t *state;
    sfBool *resolution_changed;
    sfVector2i mouse_pos;
} settings_interface_context_t;

typedef struct context_init_params_s {
    sfRenderWindow **window;
    sfEvent event;
    settings_t *settings;
    sfBool *is_open;
    game_state_t *state;
    sfBool *resolution_changed;
} context_init_params_t;

int display_settings(sfRenderWindow **window, sfEvent event,
    game_state_t *state, sfMusic *menu_music);
void destroy_settings(settings_t *settings);

void set_resolution(sfRenderWindow **window, sfBool high_resolution);

void update_volume(volume_t *volume, sfMusic *music, float new_volume);
void handle_slider_interaction(volume_t *volume, sfMusic *music,
    sfVector2i mouse_pos);

sfBool handle_system_events(settings_event_context_t *ctx);
void handle_mouse_events(settings_event_context_t *ctx);
void process_settings_events(settings_event_context_t *ctx);
void handle_mouse_button_release(sfEvent event, settings_t *settings);

int init_volume_controls(volume_t *volume);
void free_volume_controls(volume_t *volume);
void handle_mouse_interactions(settings_interface_context_t *ctx);
void process_ui_and_events(settings_interface_context_t *ctx);
int init_background(background_t *background);
void update_button_states(settings_t *settings, sfVector2i mouse_pos);
sfVector2f convert_mouse_position(sfRenderWindow *window,
    sfVector2i mouse_pos);

#endif
