/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game_events
*/

#include "wolf.h"
#include "weapon.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void manage_resize_window(weapon_t *weapon, sfEvent *event,
    game_context_t *ctx)
{
    sfVector2f center = {0};
    sfVector2f size = {0};

    if (event->type == sfEvtResized) {
        center = (sfVector2f){event->size.width / 2.0f,
            event->size.height / 2.0f};
        size = (sfVector2f){event->size.width, event->size.height};
        sfView_setCenter(ctx->game_view, center);
        sfView_setSize(ctx->game_view, size);
        update_weapon_position(weapon, ctx->window);
    }
    return;
}

static void manage_flashlight(sfEvent *event, game_context_t *ctx)
{
    if (event->type == sfEvtKeyReleased && event->key.code == sfKeyL)
        ctx->light_state = !ctx->light_state;
}

level_ctx_t init_level(sfRenderWindow *window, game_state_t *state,
    fps_info_t *fps_info, game_context_t *ctx)
{
    level_ctx_t level = {0};

    level.window = window;
    level.state = state;
    level.fps_info = fps_info;
    level.ctx = ctx;
    level.map_path = NULL;
    return level;
}

event_context_t init_event_context(game_context_t *ctx,
    weapon_t *weapon)
{
    event_context_t context = {0};

    context.event = &ctx->event;
    context.state = ctx->state;
    context.ctx = ctx;
    context.player = ctx->game_data.player;
    context.weapon = weapon;
    return context;
}

game_update_context_t init_update_context(game_context_t *ctx,
    player_t *player, weapon_t *weapon)
{
    game_update_context_t update_ctx = {0};

    update_ctx.ctx = ctx;
    update_ctx.player = player;
    update_ctx.weapon = weapon;
    update_ctx.delta_time = 0.0f;
    return update_ctx;
}

static enemy_t *find_enemy_under_crosshair(game_context_t *ctx)
{
    enemy_t *best_target = NULL;
    float best_distance = FLT_MAX;
    target_params_t params = {
        .crosshair = get_crosshair_position(ctx->window),
        .win_size = sfRenderWindow_getSize(ctx->window),
        .tolerance = 100.0f
    };

    if (!ctx || !ctx->window || !ctx->game_data.enemies)
        return NULL;
    for (enemy_t *enemy = ctx->game_data.enemies; enemy; enemy = enemy->next) {
        if (is_valid_target(enemy, ctx->game_data.player, &params)) {
            update_best_target(enemy, ctx->game_data.player,
            &best_target, &best_distance);
        }
    }
    return best_target;
}

static void process_shoot_event(event_context_t *context, sfVector2i mousePos)
{
    enemy_t *target = NULL;

    player_shoot_at_position(context->player,
        &context->ctx->game_data.projectiles, mousePos);
    play_shoot_sound(context->ctx->shoot_sound);
    weapon_fire(context->weapon);
    target = find_enemy_under_crosshair(context->ctx);
    if (target) {
        update_enemy_health(target, 50);
        if (target->state == ENEMY_STATE_DYING)
            context->ctx->game_data.enemy_count--;
    }
}

void analyse_events(event_context_t *context)
{
    sfVector2i mousePos = {0};

    while (sfRenderWindow_pollEvent(context->ctx->window, context->event)) {
        if (context->event->type == sfEvtClosed)
            sfRenderWindow_close(context->ctx->window);
        if (sfKeyboard_isKeyPressed(sfKeyEscape))
            *(context->state) = MENU;
        if (context->event->type == sfEvtMouseButtonPressed &&
            context->event->mouseButton.button == sfMouseLeft) {
            mousePos = sfMouse_getPositionRenderWindow(context->ctx->window);
            process_shoot_event(context, mousePos);
        }
        manage_resize_window(context->weapon, context->event, context->ctx);
        manage_flashlight(context->event, context->ctx);
    }
}
