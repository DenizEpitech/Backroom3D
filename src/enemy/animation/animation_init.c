/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** animation_init
*/

#include "enemy.h"
#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>

animation_config_t get_animation_config(void)
{
    animation_config_t config = {
        .idle_frames = 1,
        .patrol_frames = 4,
        .chase_frames = 4,
        .attack_frames = 3,
        .dying_frames = 5,
        .idle_time = 0.5f,
        .patrol_time = 0.3f,
        .chase_time = 0.2f,
        .attack_time = 0.4f,
        .dying_time = 0.3f
    };

    return config;
}

static int init_animation_properties(enemy_animation_set_t *set,
    animation_params_t params)
{
    if (!set)
        return EXIT_ERROR;
    set->frames = malloc(params.frame_count * sizeof(enemy_animation_frame_t));
    if (!set->frames)
        return EXIT_ERROR;
    set->frame_count = params.frame_count;
    set->current_frame = 0;
    set->frame_time = params.frame_time;
    set->elapsed_time = 0.0f;
    set->animation_clock = sfClock_create();
    if (!set->animation_clock) {
        free(set->frames);
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

static int init_animation_frames(enemy_animation_set_t *set,
    animation_params_t params)
{
    int i = 0;

    for (i = 0; i < params.frame_count; i++) {
        if (load_frame_texture(&set->frames[i], params, i) != EXIT_SUCCESS) {
            cleanup_partial_frames(set->frames, i);
            free(set->frames);
            sfClock_destroy(set->animation_clock);
            return EXIT_ERROR;
        }
        if (create_frame_sprite(&set->frames[i]) != EXIT_SUCCESS) {
            cleanup_partial_frames(set->frames, i);
            sfTexture_destroy(set->frames[i].texture);
            free(set->frames);
            sfClock_destroy(set->animation_clock);
            return EXIT_ERROR;
        }
    }
    return EXIT_SUCCESS;
}

static int init_animation_set(enemy_animation_set_t *set,
    animation_setup_t setup)
{
    animation_params_t params = {0};

    params.state = setup.state;
    params.direction = setup.direction;
    params.frame_count = setup.frame_count;
    params.frame_time = setup.frame_time;
    if (init_animation_properties(set, params) != EXIT_SUCCESS)
        return EXIT_ERROR;
    if (init_animation_frames(set, params) != EXIT_SUCCESS)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

static int init_idle_patrol_animations(enemy_t *enemy,
    animation_config_t config)
{
    animation_setup_t setup = {0};

    setup.direction = DIRECTION_FRONT;
    setup.state = ENEMY_STATE_IDLE;
    setup.frame_count = config.idle_frames;
    setup.frame_time = config.idle_time;
    if (init_animation_set(&enemy->animations.idle,
        setup) != EXIT_SUCCESS)
        return EXIT_ERROR;
    setup.state = ENEMY_STATE_PATROL;
    setup.frame_count = config.patrol_frames;
    setup.frame_time = config.patrol_time;
    if (init_animation_set(&enemy->animations.patrol,
        setup) != EXIT_SUCCESS) {
        cleanup_enemy_animations(&enemy->animations);
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

static int init_chase_animation(enemy_t *enemy,
    animation_config_t config)
{
    animation_setup_t setup = {0};

    setup.direction = DIRECTION_FRONT;
    setup.state = ENEMY_STATE_CHASE;
    setup.frame_count = config.chase_frames;
    setup.frame_time = config.chase_time;
    if (init_animation_set(&enemy->animations.chase,
        setup) != EXIT_SUCCESS) {
        cleanup_enemy_animations(&enemy->animations);
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

int init_basic_animations(enemy_t *enemy,
    animation_config_t config)
{
    if (init_idle_patrol_animations(enemy, config) != EXIT_SUCCESS)
        return EXIT_ERROR;
    if (init_chase_animation(enemy, config) != EXIT_SUCCESS)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

int init_combat_animations(enemy_t *enemy,
    animation_config_t config)
{
    animation_setup_t setup = {0};

    setup.direction = DIRECTION_FRONT;
    setup.state = ENEMY_STATE_ATTACK;
    setup.frame_count = config.attack_frames;
    setup.frame_time = config.attack_time;
    if (init_animation_set(&enemy->animations.attack,
        setup) != EXIT_SUCCESS) {
        cleanup_enemy_animations(&enemy->animations);
        return EXIT_ERROR;
    }
    setup.state = ENEMY_STATE_DYING;
    setup.frame_count = config.dying_frames;
    setup.frame_time = config.dying_time;
    if (init_animation_set(&enemy->animations.dying,
        setup) != EXIT_SUCCESS) {
        cleanup_enemy_animations(&enemy->animations);
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}
