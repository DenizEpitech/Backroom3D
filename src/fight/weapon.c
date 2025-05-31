/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** weapon
*/

#include "weapon.h"
#include <stdlib.h>
#include <stdio.h>

static void init_state_properties(weapon_state_t *state)
{
    state->state = WEAPON_IDLE;
    state->cooldown = 0.0f;
    state->cooldown_time = 0.3f;
}

static void init_display_properties(weapon_display_t *display,
    float win_width, float win_height)
{
    display->scale = (sfVector2f){3.0f, 3.0f};
    display->position = (sfVector2f){win_width / 2.0f, win_height + 50.0f};
}

static weapon_t *allocate_weapon_struct(float win_width, float win_height)
{
    weapon_t *weapon = malloc(sizeof(weapon_t));

    if (!weapon)
        return NULL;
    init_animation_properties(&weapon->animation);
    init_state_properties(&weapon->state);
    init_display_properties(&weapon->display, win_width, win_height);
    if (allocate_animation_resources(&weapon->animation) != EXIT_SUCCESS) {
        destroy_weapon(weapon);
        return NULL;
    }
    return weapon;
}

weapon_t *init_weapon(float window_width, float window_height)
{
    weapon_t *weapon = allocate_weapon_struct(window_width, window_height);

    if (!weapon)
        return NULL;
    if (load_weapon_textures(weapon) != EXIT_SUCCESS) {
        destroy_weapon(weapon);
        return NULL;
    }
    if (create_weapon_sprites(weapon) != EXIT_SUCCESS) {
        destroy_weapon(weapon);
        return NULL;
    }
    configure_weapon_sprites(weapon);
    return weapon;
}

void update_weapon(weapon_t *weapon, float delta_time)
{
    float seconds = 0.0f;

    if (!weapon || !weapon->animation.animation_clock)
        return;
    if (weapon->state.cooldown > 0.0f) {
        weapon->state.cooldown -= delta_time;
        if (weapon->state.cooldown < 0.0f)
            weapon->state.cooldown = 0.0f;
    }
    if (weapon->state.state == WEAPON_FIRING) {
        seconds = sfTime_asSeconds(sfClock_getElapsedTime
            (weapon->animation.animation_clock));
        if (seconds >= weapon->animation.frame_time)
            advance_weapon_animation(weapon);
    }
}

void weapon_fire(weapon_t *weapon)
{
    if (!weapon || weapon->state.cooldown > 0.0f)
        return;
    weapon->state.state = WEAPON_FIRING;
    weapon->animation.current_frame = 0;
    weapon->state.cooldown = weapon->state.cooldown_time;
    sfClock_restart(weapon->animation.animation_clock);
    return;
}

void render_weapon(sfRenderWindow *window, weapon_t *weapon)
{
    if (!window || !weapon || !weapon->animation.sprites ||
        weapon->animation.current_frame < 0 ||
        weapon->animation.current_frame >= weapon->animation.total_frames)
        return;
    sfRenderWindow_drawSprite(window, weapon->animation.sprites
        [weapon->animation.current_frame], NULL);
    return;
}

void destroy_weapon(weapon_t *weapon)
{
    if (!weapon)
        return;
    if (weapon->animation.sprites && weapon->animation.textures)
        destroy_animation_resources(&weapon->animation);
    if (weapon->animation.sprites)
        free(weapon->animation.sprites);
    if (weapon->animation.textures)
        free(weapon->animation.textures);
    if (weapon->animation.animation_clock)
        sfClock_destroy(weapon->animation.animation_clock);
    free(weapon);
    return;
}
