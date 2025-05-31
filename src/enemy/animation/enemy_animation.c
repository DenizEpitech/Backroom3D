/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_animation
*/

#include "enemy.h"
#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int init_enemy_animations(enemy_t *enemy)
{
    animation_config_t config = {0};

    if (!enemy)
        return EXIT_ERROR;
    config = get_animation_config();
    if (init_basic_animations(enemy, config) != EXIT_SUCCESS)
        return EXIT_ERROR;
    if (init_combat_animations(enemy, config) != EXIT_SUCCESS)
        return EXIT_ERROR;
    enemy->animations.current = &enemy->animations.idle;
    enemy->direction = DIRECTION_FRONT;
    return EXIT_SUCCESS;
}

static void update_animation_set(enemy_animation_set_t *set, float delta_time)
{
    if (!set || !set->animation_clock)
        return;
    set->elapsed_time += delta_time;
    if (set->elapsed_time >= set->frame_time) {
        set->current_frame = (set->current_frame + 1) % set->frame_count;
        set->elapsed_time = 0.0f;
    }
}

void update_enemy_animation(enemy_t *enemy, float delta_time)
{
    if (!enemy || !enemy->animations.current)
        return;
    if (enemy->state != enemy->previous_state) {
        set_enemy_animation_state(enemy, enemy->state);
        enemy->previous_state = enemy->state;
    }
    update_animation_set(enemy->animations.current, delta_time);
}

static enemy_animation_set_t *select_animation_for_state(enemy_t *enemy,
    enemy_state_t new_state)
{
    if (!enemy)
        return NULL;
    switch (new_state) {
        case ENEMY_STATE_IDLE:
            return &enemy->animations.idle;
        case ENEMY_STATE_PATROL:
            return &enemy->animations.patrol;
        case ENEMY_STATE_CHASE:
        case ENEMY_STATE_ATTACK:
            return &enemy->animations.attack;
        case ENEMY_STATE_DYING:
        case ENEMY_STATE_DEAD:
            return &enemy->animations.dying;
        default:
            return &enemy->animations.idle;
    }
}

static void reset_animation_to_start(enemy_animation_set_t *animation_set)
{
    if (!animation_set)
        return;
    animation_set->current_frame = 0;
    animation_set->elapsed_time = 0.0f;
    if (animation_set->animation_clock)
        sfClock_restart(animation_set->animation_clock);
}

void set_enemy_animation_state(enemy_t *enemy, enemy_state_t new_state)
{
    enemy_animation_set_t *new_animation = NULL;

    if (!enemy)
        return;
    enemy->state = new_state;
    new_animation = select_animation_for_state(enemy, new_state);
    if (!new_animation)
        return;
    enemy->animations.current = new_animation;
    reset_animation_to_start(enemy->animations.current);
}

void cleanup_animation_set(enemy_animation_set_t *set)
{
    int i = 0;

    if (!set || !set->frames)
        return;
    for (i = 0; i < set->frame_count; i++) {
        if (set->frames[i].sprite)
            sfSprite_destroy(set->frames[i].sprite);
        if (set->frames[i].texture)
            sfTexture_destroy(set->frames[i].texture);
    }
    free(set->frames);
    set->frames = NULL;
    if (set->animation_clock)
        sfClock_destroy(set->animation_clock);
}

void cleanup_enemy_animations(enemy_animations_t *animations)
{
    if (!animations)
        return;
    cleanup_animation_set(&animations->idle);
    cleanup_animation_set(&animations->patrol);
    cleanup_animation_set(&animations->chase);
    cleanup_animation_set(&animations->attack);
    cleanup_animation_set(&animations->dying);
    animations->current = NULL;
}
