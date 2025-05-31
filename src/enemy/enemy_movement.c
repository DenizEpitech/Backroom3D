/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_movement
*/

#include "enemy.h"
#include "wolf.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void update_enemy_pursuit(enemy_t *enemy, player_t *player,
    float delta_time)
{
    float actual_speed = ENEMY_CHASE_SPEED;
    float angle_to_player = atan2f(player->y - enemy->y,
        player->x - enemy->x);
    float move_x = enemy->x + cos(angle_to_player)
        * actual_speed * delta_time;
    float move_y = enemy->y + sin(angle_to_player)
        * actual_speed * delta_time;

    if (can_move_to_position(enemy->data, move_x, move_y)) {
        enemy->x = move_x;
        enemy->y = move_y;
    } else {
        move_x = enemy->x + cos(angle_to_player) * actual_speed * delta_time;
        if (can_move_to_position(enemy->data, move_x, enemy->y))
            enemy->x = move_x;
        move_y = enemy->y + sin(angle_to_player) * actual_speed * delta_time;
        if (can_move_to_position(enemy->data, enemy->x, move_y))
            enemy->y = move_y;
    }
    enemy->direction = calculate_enemy_direction(enemy, player);
}

static void update_direction_timer(enemy_t *enemy, float delta_time)
{
    static float dir_change_timer = 0.0f;

    dir_change_timer += delta_time;
    if (dir_change_timer > ENEMY_DIR_CHANGE_TIME) {
        dir_change_timer = 0.0f;
        enemy->patrol.current_angle +=
        ((float)(rand() % 100) / 100.0f - 0.5f) * M_PI;
    }
    enemy->patrol.current_angle += 0.2f * delta_time;
    if (enemy->patrol.current_angle > 2 * M_PI)
        enemy->patrol.current_angle -= 2 * M_PI;
}

static int try_move_enemy(enemy_t *enemy, float angle, float delta_time)
{
    float patrol_speed = ENEMY_PATROL_SPEED;
    float move_x = enemy->x + cos(angle) * patrol_speed * delta_time;
    float move_y = enemy->y + sin(angle) * patrol_speed * delta_time;

    if (can_move_to_position(enemy->data, move_x, move_y)) {
        enemy->x = move_x;
        enemy->y = move_y;
        return 1;
    }
    return 0;
}

static void handle_patrol_bounds(enemy_t *enemy, float delta_time)
{
    float dx = enemy->x - enemy->patrol.start_x;
    float dy = enemy->y - enemy->patrol.start_y;
    float dist_from_start = sqrtf(dx * dx + dy * dy);
    float angle_to_start = 0.0f;

    if (dist_from_start > enemy->patrol.patrol_radius) {
        angle_to_start = atan2f(enemy->patrol.start_y - enemy->y,
            enemy->patrol.start_x - enemy->x);
        try_move_enemy(enemy, angle_to_start, delta_time);
    }
}

void update_enemy_patrol(enemy_t *enemy, float delta_time)
{
    if (!enemy || !enemy->data)
        return;
    update_direction_timer(enemy, delta_time);
    if (!try_move_enemy(enemy, enemy->patrol.current_angle, delta_time))
        enemy->patrol.current_angle += M_PI / 2;
    handle_patrol_bounds(enemy, delta_time);
}
