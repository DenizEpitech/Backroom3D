/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** init_ennemi
*/

#include "enemy.h"
#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void init_patrol_settings(enemy_t *enemy)
{
    enemy->patrol.start_x = enemy->x;
    enemy->patrol.start_y = enemy->y;
    enemy->patrol.patrol_radius = 3 * TILE_SIZE;
    enemy->patrol.current_angle = (float)(rand() % 628) / 100.0f;
    enemy->patrol.patrol_speed = 0.5f;
}

int initialize_enemy_components(enemy_t *enemy)
{
    init_enemy_stats(&enemy->stats, enemy->type);
    if (init_enemy_animations(enemy) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void init_random_seed(void)
{
    static int rand_seeded = 0;

    if (!rand_seeded) {
        srand(time(NULL));
        rand_seeded = 1;
    }
}

static void set_default_stats(enemy_stats_t *stats)
{
    stats->health = 100;
    stats->max_health = 100;
    stats->speed = 1.0f;
    stats->damage = 10;
    stats->detection_range = 300.0f;
    stats->attack_range = 180.0f;
}

static void set_soldier_stats(enemy_stats_t *stats)
{
    stats->health = 100;
    stats->max_health = 100;
    stats->speed = 1.0f;
    stats->damage = 10;
    stats->detection_range = 300.0f;
    stats->attack_range = 180.0f;
}

static void apply_enemy_type_stats(enemy_stats_t *stats,
    enemy_type_t type)
{
    switch (type) {
        case ENEMY_TYPE_SOLDIER:
            set_soldier_stats(stats);
            break;
        default:
            set_default_stats(stats);
            break;
    }
}

void init_enemy_stats(enemy_stats_t *stats, enemy_type_t type)
{
    if (!stats)
        return;
    init_random_seed();
    apply_enemy_type_stats(stats, type);
}

enemy_t *allocate_enemy(void)
{
    enemy_t *enemy = malloc(sizeof(enemy_t));

    if (!enemy)
        return NULL;
    enemy->next = NULL;
    enemy->data = NULL;
    return enemy;
}
