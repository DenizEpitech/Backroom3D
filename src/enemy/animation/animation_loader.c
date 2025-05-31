/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** animation_loader
*/

#include "enemy.h"
#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>

static const char *direction_strings[] = {
    [DIRECTION_FRONT] = "devant",
    [DIRECTION_BACK] = "derriere",
    [DIRECTION_RIGHT] = "droite",
    [DIRECTION_LEFT] = "gauche",
    [DIRECTION_RIGHT_FRONT] = "droite_devant",
    [DIRECTION_LEFT_FRONT] = "devant_gauche",
    [DIRECTION_RIGHT_BACK] = "derriere_droite",
    [DIRECTION_LEFT_BACK] = "derriere_gauche"
};

static char *build_directional_path(enemy_direction_t direction, int frame)
{
    const char *direction_str = NULL;
    char *path = NULL;
    int path_length = 0;
    int frame_num = frame % 4;

    direction_str = (direction >= 0 && direction < 8) ?
        direction_strings[direction] : "devant";
    path_length = snprintf(NULL, 0,
        "ressources/game/guard/%s/%s%d.png",
        direction_str, direction_str, frame_num) + 1;
    path = malloc(path_length * sizeof(char));
    if (!path)
        return NULL;
    snprintf(path, path_length,
        "ressources/game/guard/%s/%s%d.png",
        direction_str, direction_str, frame_num);
    return path;
}

static const char *get_state_format(enemy_state_t state,
    int frame, int *frame_num)
{
    switch (state) {
        case ENEMY_STATE_ATTACK:
            *frame_num = frame % 3;
            return "ressources/game/guard/attack/attack%d.png";
        case ENEMY_STATE_DYING:
        case ENEMY_STATE_DEAD:
            *frame_num = frame % 5;
            return "ressources/game/guard/mort/mort%d.png";
        default:
            return NULL;
    }
}

static char *allocate_formatted_path(const char *format, int frame_num)
{
    char *path = NULL;
    int path_length = 0;

    if (!format)
        return NULL;
    path_length = snprintf(NULL, 0, format, frame_num) + 1;
    path = malloc(path_length * sizeof(char));
    if (!path)
        return NULL;
    snprintf(path, path_length, format, frame_num);
    return path;
}

static char *build_state_path(enemy_state_t state, int frame)
{
    const char *format = NULL;
    int frame_num = 0;

    format = get_state_format(state, frame, &frame_num);
    return allocate_formatted_path(format, frame_num);
}

static char *create_animation_path(enemy_state_t state,
    enemy_direction_t direction, int frame)
{
    if (state == ENEMY_STATE_ATTACK ||
        state == ENEMY_STATE_DYING ||
        state == ENEMY_STATE_DEAD) {
        return build_state_path(state, frame);
    }
    return build_directional_path(direction, frame);
}

void cleanup_partial_frames(enemy_animation_frame_t *frames, int count)
{
    int j = 0;

    for (j = 0; j < count; j++) {
        if (frames[j].sprite)
            sfSprite_destroy(frames[j].sprite);
        if (frames[j].texture)
            sfTexture_destroy(frames[j].texture);
    }
}

int load_frame_texture(enemy_animation_frame_t *frame,
    animation_params_t params, int i)
{
    char *path = NULL;

    path = create_animation_path(params.state, params.direction, i);
    if (!path)
        return EXIT_ERROR;
    frame->texture = sfTexture_createFromFile(path, NULL);
    free(path);
    if (!frame->texture) {
        frame->texture = sfTexture_createFromFile
        ("ressources/game/ennemy.png", NULL);
        if (!frame->texture)
            return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

int create_frame_sprite(enemy_animation_frame_t *frame)
{
    sfVector2u texture_size = {0};

    frame->sprite = sfSprite_create();
    if (!frame->sprite)
        return EXIT_ERROR;
    sfSprite_setTexture(frame->sprite, frame->texture, sfTrue);
    texture_size = sfTexture_getSize(frame->texture);
    sfSprite_setOrigin(frame->sprite,
        (sfVector2f){texture_size.x / 2, texture_size.y / 2});
    return EXIT_SUCCESS;
}
