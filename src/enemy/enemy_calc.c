/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d
** File description:
** enemy_calc.c
*/

#include "wolf.h"
#include "weapon.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static bool is_enemy_targetable(enemy_t *enemy)
{
    return enemy->state != ENEMY_STATE_DYING &&
        enemy->state != ENEMY_STATE_DEAD;
}

static bool is_enemy_in_fov(float angle)
{
    return fabsf(angle) <= FOV / 2;
}

static float calculate_screen_position(float angle,
    sfVector2u win_size)
{
    return (0.5f + angle / FOV) * win_size.x;
}

float calculate_enemy_distance(enemy_t *enemy, player_t *player)
{
    float dx = enemy->x - player->x;
    float dy = enemy->y - player->y;

    return sqrt(dx * dx + dy * dy);
}

float calculate_enemy_angle(enemy_t *enemy, player_t *player)
{
    float dx = enemy->x - player->x;
    float dy = enemy->y - player->y;
    float angle = atan2f(dy, dx) - player->angle;

    while (angle > M_PI)
        angle -= 2 * M_PI;
    while (angle < - M_PI)
        angle += 2 * M_PI;
    return angle;
}

static bool is_enemy_under_crosshair(float screen_x,
    sfVector2i mousePos, float tolerance)
{
    return fabsf(screen_x - mousePos.x) < tolerance;
}

static bool is_better_target(float distance, float best_distance)
{
    return distance < best_distance;
}

int is_enemy_visible(enemy_t *enemy, player_t *player,
    game_data_t *game_data)
{
    float angle = calculate_enemy_angle(enemy, player);
    float distance = calculate_enemy_distance(enemy, player);
    wall_hit_t *hit = NULL;
    int visible = 0;

    if (enemy->state == ENEMY_STATE_DEAD)
        return 0;
    if (distance > 10000.0f)
        return 0;
    if (fabsf(angle) > FOV / 2)
        return 0;
    hit = cast_single_ray(game_data, player->angle + angle);
    if (!hit)
        return 0;
    visible = hit->distance > distance;
    free(hit);
    return visible;
}

bool is_valid_target(enemy_t *enemy, player_t *player,
    target_params_t *params)
{
    float angle = 0.0f;
    float screen_x = 0.0f;

    if (!is_enemy_targetable(enemy))
        return false;
    angle = calculate_enemy_angle(enemy, player);
    if (!is_enemy_in_fov(angle))
        return false;
    screen_x = calculate_screen_position(angle, params->win_size);
    return is_enemy_under_crosshair(screen_x, params->crosshair,
        params->tolerance);
}

void update_best_target(enemy_t *enemy, player_t *player,
    enemy_t **best_target, float *best_distance)
{
    float distance = calculate_enemy_distance(enemy, player);

    if (is_better_target(distance, *best_distance)) {
        *best_target = enemy;
        *best_distance = distance;
    }
}
