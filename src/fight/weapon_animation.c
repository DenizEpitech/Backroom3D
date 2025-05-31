/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** weapon_animation
*/

#include "weapon.h"
#include <stdlib.h>
#include <stdio.h>

void init_animation_properties(weapon_animation_t *animation)
{
    animation->total_frames = 5;
    animation->current_frame = 0;
    animation->frame_time = 0.07f;
}

int allocate_animation_resources(weapon_animation_t *animation)
{
    animation->textures = malloc
    (animation->total_frames * sizeof(sfTexture*));
    animation->sprites = malloc
    (animation->total_frames * sizeof(sfSprite*));
    if (!animation->textures || !animation->sprites)
        return EXIT_FAILURE;
    for (int i = 0; i < animation->total_frames; i++) {
        animation->textures[i] = NULL;
        animation->sprites[i] = NULL;
    }
    animation->animation_clock = sfClock_create();
    if (!animation->animation_clock)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int load_weapon_textures(weapon_t *weapon)
{
    char *texture_path = NULL;

    for (int i = 0; i < weapon->animation.total_frames; i++) {
        texture_path = create_texture_path(i);
        if (!texture_path)
            return EXIT_FAILURE;
        weapon->animation.textures[i] =
        sfTexture_createFromFile(texture_path, NULL);
        free(texture_path);
        if (!weapon->animation.textures[i]) {
            fprintf(stderr, "Failed to load texture %d\n", i);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

void destroy_animation_resources(weapon_animation_t *animation)
{
    if (!animation)
        return;
    for (int i = 0; i < animation->total_frames; i++) {
        if (animation->sprites[i]) {
            sfSprite_destroy(animation->sprites[i]);
            animation->sprites[i] = NULL;
        }
        if (animation->textures[i]) {
            sfTexture_destroy(animation->textures[i]);
            animation->textures[i] = NULL;
        }
    }
}

void advance_weapon_animation(weapon_t *weapon)
{
    weapon->animation.current_frame++;
    sfClock_restart(weapon->animation.animation_clock);
    if (weapon->animation.current_frame >=
        weapon->animation.total_frames) {
        weapon->animation.current_frame = 0;
        weapon->state.state = WEAPON_IDLE;
    }
}
