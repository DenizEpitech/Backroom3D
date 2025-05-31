/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_behavior
*/

#include "enemy.h"
#include "wolf.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void enemy_shoot_at_player(enemy_t *enemy, player_t *player,
    projectile_t **projectile_list)
{
    project_info_t info = {0};
    float angle_to_player = 0.0f;

    if (!enemy || !player || !projectile_list)
        return;
    info.x = enemy->x;
    info.y = enemy->y;
    angle_to_player = atan2f(player->y - enemy->y, player->x - enemy->x);
    info.dir.x = cos(angle_to_player);
    info.dir.y = sin(angle_to_player);
    info.speed = 3.0f;
    info.damage = enemy->stats.damage;
    add_projectile(projectile_list, info);
    if (player->context && player->context->shoot_sound)
        play_shoot_sound(player->context->shoot_sound);
}

static void handle_patrol_state(enemy_t *enemy, player_t *player,
    float distance_to_player, float delta_time)
{
    update_enemy_patrol(enemy, delta_time);
    if (distance_to_player < enemy->stats.detection_range &&
        is_enemy_visible(enemy, player, enemy->data)) {
        enemy->state = ENEMY_STATE_CHASE;
        set_enemy_animation_state(enemy, ENEMY_STATE_CHASE);
    }
}

static void handle_chase_state(enemy_t *enemy, player_t *player,
    float distance_to_player, float delta_time)
{
    update_enemy_pursuit(enemy, player, delta_time);
    if (distance_to_player < enemy->stats.attack_range) {
        enemy->state = ENEMY_STATE_ATTACK;
        set_enemy_animation_state(enemy, ENEMY_STATE_ATTACK);
    } else if (distance_to_player > enemy->stats.detection_range * 1.5f) {
        enemy->state = ENEMY_STATE_PATROL;
        set_enemy_animation_state(enemy, ENEMY_STATE_PATROL);
    }
}

static void handle_attack_state(enemy_t *enemy, player_t *player,
    float distance_to_player, float delta_time)
{
    static float shoot_cooldown[100] = {0};
    int id = ((int)(enemy->x + enemy->y)) % 100;

    update_enemy_pursuit(enemy, player, delta_time *
        ENEMY_ATTACK_SPEED / ENEMY_CHASE_SPEED);
    if (enemy->animations.current != &enemy->animations.attack)
        set_enemy_animation_state(enemy, ENEMY_STATE_ATTACK);
    if (distance_to_player <= enemy->stats.attack_range) {
        shoot_cooldown[id] -= delta_time;
        if (shoot_cooldown[id] <= 0.0f) {
            enemy_shoot_at_player(enemy, player, &enemy->data->projectiles);
            shoot_cooldown[id] = ENEMY_SHOOT_COOLDOWN;
        }
    } else {
        enemy->state = (distance_to_player < enemy->stats.detection_range) ?
            ENEMY_STATE_CHASE : ENEMY_STATE_PATROL;
        set_enemy_animation_state(enemy, enemy->state);
    }
}

static void handle_hurt_state(enemy_t *enemy, player_t *player,
    float distance_to_player, float delta_time)
{
    static float hurt_timers[100] = {0};
    int id = ((int)(enemy->x + enemy->y)) % 100;
    float angle_from_player = atan2f(enemy->y - player->y,
        enemy->x - player->x);
    float back_x = enemy->x +
    cos(angle_from_player) * delta_time * ENEMY_HURT_RECOIL;
    float back_y = enemy->y +
    sin(angle_from_player) * delta_time * ENEMY_HURT_RECOIL;

    if (can_move_to_position(enemy->data, back_x, back_y)) {
        enemy->x = back_x;
        enemy->y = back_y;
    }
    hurt_timers[id] += delta_time;
    if (hurt_timers[id] >= 0.5f) {
        hurt_timers[id] = 0.0f;
        enemy->state = (distance_to_player < enemy->stats.detection_range) ?
            ENEMY_STATE_CHASE : ENEMY_STATE_PATROL;
        set_enemy_animation_state(enemy, enemy->state);
    }
}

static void handle_enemy_movement_states(enemy_t *enemy, player_t *player,
    float distance_to_player, float delta_time)
{
    switch (enemy->state) {
        case ENEMY_STATE_IDLE:
        case ENEMY_STATE_PATROL:
            handle_patrol_state(enemy, player,
                distance_to_player, delta_time);
            break;
        case ENEMY_STATE_CHASE:
            handle_chase_state(enemy, player,
                distance_to_player, delta_time);
            break;
        default:
            break;
    }
}

static void handle_enemy_combat_states(enemy_t *enemy, player_t *player,
    float distance_to_player, float delta_time)
{
    switch (enemy->state) {
        case ENEMY_STATE_ATTACK:
            handle_attack_state(enemy, player,
                distance_to_player, delta_time);
            break;
        case ENEMY_STATE_HURT:
            handle_hurt_state(enemy, player,
                distance_to_player, delta_time);
            break;
        default:
            if (enemy->state != ENEMY_STATE_IDLE &&
                enemy->state != ENEMY_STATE_PATROL &&
                enemy->state != ENEMY_STATE_CHASE) {
                enemy->state = ENEMY_STATE_PATROL;
                set_enemy_animation_state(enemy, ENEMY_STATE_PATROL);
            }
            break;
    }
}

void update_enemy_behavior(enemy_t *enemy, player_t *player,
    float delta_time)
{
    float distance_to_player = 0.0f;

    if (!enemy || !player || !enemy->data)
        return;
    if (enemy->state == ENEMY_STATE_DYING ||
        enemy->state == ENEMY_STATE_DEAD)
        return;
    distance_to_player = calculate_enemy_distance(enemy, player);
    enemy->direction = calculate_enemy_direction(enemy, player);
    handle_enemy_movement_states(enemy, player,
        distance_to_player, delta_time);
    handle_enemy_combat_states(enemy, player, distance_to_player, delta_time);
    update_enemy_animation(enemy, delta_time);
}
