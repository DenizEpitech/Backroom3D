/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d-donata.contant
** File description:
** manage_health.c
*/

#include "wolf.h"
#include "enemy.h"
#include <SFML/Graphics.h>
#include <math.h>
#include <stdlib.h>

int calculate_health_sprite_index(float health_percentage)
{
    if (health_percentage > 0.8f)
        return 0;
    if (health_percentage > 0.6f)
        return 1;
    if (health_percentage > 0.4f)
        return 2;
    if (health_percentage > 0.2f)
        return 3;
    else
        return 4;
}

int is_enemy_nearby(enemy_t *enemy_list, player_t *player)
{
    enemy_t *current = enemy_list;
    float distance = 0.0f;

    while (current) {
        distance = calculate_enemy_distance(current, player);
        if (distance <= 200.0f)
            return 1;
        current = current->next;
    }
    return EXIT_SUCCESS;
}

void handle_damage_and_display(health_sprite_system_t *system,
    int player_under_attack, float *damage_cooldown)
{
    if (player_under_attack && *damage_cooldown <= 0.0f) {
        system->current_health -= 20;
        *damage_cooldown = 1.0f;
        if (system->current_health < 0)
            system->current_health = 0;
        system->is_visible = 1;
        system->display_timer = 5.0f;
    }
}

void update_display_and_sprite(health_sprite_system_t *system)
{
    float health_percentage = 0.0f;

    if (system->current_health < 100) {
        system->is_visible = 1;
        system->display_timer = 999.0f;
    }
    health_percentage = (float)system->current_health / system->max_health;
    system->current_sprite_index =
        calculate_health_sprite_index(health_percentage);
}

static void destroy_all_sprites(sfSprite **sprites)
{
    int i = 0;

    for (i = 0; i < 5; i++) {
        if (sprites[i])
            sfSprite_destroy(sprites[i]);
    }
    free(sprites);
}

static void destroy_all_textures(sfTexture **textures)
{
    int i = 0;

    for (i = 0; i < 5; i++) {
        if (textures[i])
            sfTexture_destroy(textures[i]);
    }
    free(textures);
}

void destroy_health_sprite_system(health_sprite_system_t *system)
{
    if (!system)
        return;
    if (system->health_sprites)
        destroy_all_sprites(system->health_sprites);
    if (system->health_textures)
        destroy_all_textures(system->health_textures);
    free(system);
}
