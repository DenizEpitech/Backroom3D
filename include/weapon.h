/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** weapon
*/

#ifndef WEAPON_H_
    #define WEAPON_H_
    #include <SFML/Graphics.h>
    #include "wolf.h"

typedef enum {
    WEAPON_IDLE,
    WEAPON_FIRING,
    WEAPON_RELOADING
} weapon_state_enum_t;

typedef struct weapon_animation_s {
    sfTexture **textures;
    sfSprite **sprites;
    int current_frame;
    int total_frames;
    sfClock *animation_clock;
    float frame_time;
} weapon_animation_t;

typedef struct weapon_state_s {
    weapon_state_enum_t state;
    float cooldown;
    float cooldown_time;
} weapon_state_t;

typedef struct weapon_display_s {
    sfVector2f position;
    sfVector2f scale;
} weapon_display_t;

typedef struct weapon_s {
    weapon_animation_t animation;
    weapon_state_t state;
    weapon_display_t display;
} weapon_t;

weapon_t *init_weapon(float window_width, float window_height);
void destroy_weapon(weapon_t *weapon);
void render_weapon(sfRenderWindow *window, weapon_t *weapon);
void update_weapon(weapon_t *weapon, float delta_time);
void weapon_fire(weapon_t *weapon);

void init_animation_properties(weapon_animation_t *animation);
int allocate_animation_resources(weapon_animation_t *animation);
int load_weapon_textures(weapon_t *weapon);
void destroy_animation_resources(weapon_animation_t *animation);
void advance_weapon_animation(weapon_t *weapon);

char *create_texture_path(int index);
int create_weapon_sprites(weapon_t *weapon);
void configure_weapon_sprites(weapon_t *weapon);
void switch_weapon_state(weapon_t *weapon, weapon_state_enum_t new_state);
void update_weapon_position(weapon_t *weapon, sfRenderWindow *window);

#endif
