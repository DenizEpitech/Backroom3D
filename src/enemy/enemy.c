/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy
*/

#include "enemy.h"
#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

static void set_enemy_position(enemy_t *enemy, float x, float y)
{
    enemy->x = x;
    enemy->y = y;
}

static void set_basic_properties(enemy_t *enemy, enemy_type_t type)
{
    enemy->type = type;
    enemy->state = ENEMY_STATE_PATROL;
    enemy->previous_state = ENEMY_STATE_IDLE;
    enemy->angle = 0.0f;
    enemy->direction = DIRECTION_FRONT;
}

enemy_t *create_enemy_with_type(float x, float y, enemy_type_t type)
{
    enemy_t *enemy = NULL;

    init_random_seed();
    enemy = allocate_enemy();
    if (!enemy)
        return NULL;
    set_enemy_position(enemy, x, y);
    set_basic_properties(enemy, type);
    init_patrol_settings(enemy);
    if (initialize_enemy_components(enemy) != EXIT_SUCCESS) {
        free(enemy);
        return NULL;
    }
    return enemy;
}

enemy_t *create_enemy(float x, float y)
{
    return create_enemy_with_type(x, y, ENEMY_TYPE_SOLDIER);
}

void update_enemy_health(enemy_t *enemy, int damage)
{
    if (!enemy)
        return;
    enemy->stats.health -= damage;
    if (enemy->stats.health <= 0) {
        enemy->stats.health = 0;
        enemy->state = ENEMY_STATE_DYING;
        if (enemy->animations.current != &enemy->animations.dying) {
            enemy->animations.current = &enemy->animations.dying;
            enemy->animations.current->current_frame = 0;
            enemy->animations.current->elapsed_time = 0.0f;
            sfClock_restart(enemy->animations.current->animation_clock);
        }
    } else {
        enemy->state = ENEMY_STATE_HURT;
        set_enemy_animation_state(enemy, ENEMY_STATE_HURT);
    }
}

void destroy_enemy(enemy_t *enemy)
{
    if (!enemy)
        return;
    cleanup_enemy_animations(&enemy->animations);
    free(enemy);
}
