/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d
** File description:
** enemy_states.c
*/

#include "enemy.h"
#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static enemy_direction_t get_direction_one(float angle)
{
    if (angle >= 337.5f || angle < 22.5f)
        return DIRECTION_RIGHT;
    if (angle >= 22.5f && angle < 67.5f)
        return DIRECTION_RIGHT_FRONT;
    if (angle >= 67.5f && angle < 112.5f)
        return DIRECTION_FRONT;
    if (angle >= 112.5f && angle < 157.5f)
        return DIRECTION_LEFT_FRONT;
    else
        return -1;
}

static enemy_direction_t get_direction_two(float angle)
{
    if (angle >= 157.5f && angle < 202.5f)
        return DIRECTION_LEFT;
    if (angle >= 202.5f && angle < 247.5f)
        return DIRECTION_LEFT_BACK;
    if (angle >= 247.5f && angle < 292.5f)
        return DIRECTION_BACK;
    else
        return DIRECTION_RIGHT_BACK;
}

static enemy_direction_t get_direction_from_angle(float angle)
{
    enemy_direction_t result = get_direction_one(angle);

    if (result)
        return result;
    else
        return get_direction_two(angle);
}

enemy_direction_t calculate_enemy_direction(enemy_t *enemy,
    player_t *player)
{
    float dx = player->x - enemy->x;
    float dy = player->y - enemy->y;
    float angle = atan2f(dy, dx);

    angle = angle * 180.0f / M_PI;
    if (angle < 0)
        angle += 360.0f;
    return get_direction_from_angle(angle);
}
