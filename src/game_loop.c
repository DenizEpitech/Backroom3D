/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game_loop
*/

#include "wolf.h"
#include "weapon.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static float calculate_delta_time(sfClock *delta_clock,
    sfTime *last_frame)
{
    sfTime current_time = sfClock_getElapsedTime(delta_clock);
    float delta_time = sfTime_asSeconds
        (current_time) - sfTime_asSeconds(*last_frame);

    *last_frame = current_time;
    return delta_time;
}

static bool should_transition_to_dead(enemy_t *enemy)
{
    return enemy->state == ENEMY_STATE_DYING &&
        enemy->animations.current->current_frame >=
        (enemy->animations.current->frame_count * 2 / 3);
}

static void update_dying_enemy(enemy_t *enemy)
{
    if (should_transition_to_dead(enemy))
        enemy->state = ENEMY_STATE_DEAD;
}

static void remove_dead_enemy(enemy_t **current)
{
    enemy_t *enemy = *current;

    *current = enemy->next;
    destroy_enemy(enemy);
}

static void cleanup_dead_enemies(game_data_t *game_data)
{
    enemy_t **current = &game_data->enemies;
    enemy_t *enemy = {0};

    if (!game_data || !game_data->enemies)
        return;
    while (*current) {
        enemy = *current;
        update_dying_enemy(enemy);
        if (enemy->state == ENEMY_STATE_DEAD) {
            remove_dead_enemy(current);
        } else {
            current = &enemy->next;
        }
    }
}

static void update_all_enemies(game_update_context_t *context)
{
    enemy_t *current = context->ctx->game_data.enemies;

    while (current) {
        update_enemy_animation(current, context->delta_time);
        if (current->data && current->state != ENEMY_STATE_DYING) {
            update_enemy_behavior(current, context->player,
                context->delta_time);
        }
        current = current->next;
    }
}

static void update_game_state(game_update_context_t *context)
{
    update_player(context->player);
    update_weapon(context->weapon, context->delta_time);
    if (context->ctx->health_system) {
        update_health_sprite_system_simple(context->ctx->health_system,
            context->player, context->ctx->game_data.enemies,
            context->delta_time);
    }
    update_all_enemies(context);
    cleanup_dead_enemies(&context->ctx->game_data);
    if (context->ctx->health_system
        && is_player_dead(context->ctx->health_system)) {
        display_game_over(context->ctx->window);
        *(context->ctx->state) = MENU;
    }
    if (context->ctx->fps_info)
        update_fps_display(context->ctx->fps_info);
    if (context->ctx->crosshair)
        update_crosshair(context->ctx->crosshair, context->ctx->window);
}

void process_game_frame(game_loop_resources_t *resources)
{
    float delta_time = calculate_delta_time(resources->delta_clock,
        &resources->last_frame);

    resources->update_ctx.delta_time = delta_time;
    update_game_state(&resources->update_ctx);
    analyse_events(&resources->event_ctx);
    render_game_elements(&resources->update_ctx);
}

int init_game_loop_resources(game_loop_resources_t *resources,
    game_context_t *ctx, player_t *player)
{
    sfVector2u win_size = sfRenderWindow_getSize(ctx->window);

    resources->weapon = init_weapon(win_size.x, win_size.y);
    resources->delta_clock = sfClock_create();
    if (!resources->weapon || !resources->delta_clock)
        return EXIT_ERROR;
    resources->last_frame = sfClock_getElapsedTime(resources->delta_clock);
    resources->event_ctx = init_event_context(ctx, resources->weapon);
    resources->update_ctx = init_update_context(ctx, player,
        resources->weapon);
    return EXIT_SUCCESS;
}

void cleanup_game_loop_ressources(game_loop_resources_t *resources)
{
    if (resources->weapon)
        destroy_weapon(resources->weapon);
    if (resources->delta_clock)
        sfClock_destroy(resources->delta_clock);
}
