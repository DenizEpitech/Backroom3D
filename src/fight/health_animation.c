/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** health_sprite_system
*/

#include "wolf.h"
#include "enemy.h"
#include <SFML/Graphics.h>
#include <math.h>
#include <stdlib.h>

void update_health_sprite_system_simple(health_sprite_system_t *system,
    player_t *player, enemy_t *enemy_list, float delta_time)
{
    static float damage_cooldown = 0.0f;
    int player_under_attack = 0;

    if (!system || !player)
        return;
    if (damage_cooldown > 0.0f)
        damage_cooldown -= delta_time;
    player_under_attack = is_enemy_nearby(enemy_list, player);
    handle_damage_and_display(system, player_under_attack, &damage_cooldown);
    update_display_and_sprite(system);
    if (system->display_timer > 0.0f) {
        system->display_timer -= delta_time;
        if (system->display_timer <= 0.0f &&
            system->current_health == system->max_health) {
            system->is_visible = 0;
        }
    }
}

void render_health_sprite(sfRenderWindow *window,
    health_sprite_system_t *system)
{
    sfSprite *current_sprite = {0};

    if (!system || !system->is_visible || !system->health_sprites)
        return;
    if (system->current_sprite_index < 0 || system->current_sprite_index >= 5)
        system->current_sprite_index = 0;
    current_sprite = system->health_sprites[system->current_sprite_index];
    if (!current_sprite)
        return;
    sfSprite_setPosition(current_sprite, system->position);
    sfRenderWindow_drawSprite(window, current_sprite, NULL);
}

void set_health_sprite_position(health_sprite_system_t *system,
    float x, float y)
{
    if (!system)
        return;
    system->position = (sfVector2f){x, y};
}

int is_player_dead(health_sprite_system_t *system)
{
    if (!system)
        return 0;
    return (system->current_health <= 0);
}

void apply_damage_to_player(health_sprite_system_t *system, int damage)
{
    float health_percentage = 0.0f;

    if (!system)
        return;
    system->current_health -= damage;
    if (system->current_health < 0)
        system->current_health = 0;
    system->is_visible = 1;
    system->display_timer = 5.0f;
    health_percentage = (float)system->current_health / system->max_health;
    system->current_sprite_index =
        calculate_health_sprite_index(health_percentage);
}
