/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d
** File description:
** render_elements.c
*/

#include "wolf.h"
#include "weapon.h"

static void render_game_view(game_context_t *ctx)
{
    sfRenderWindow_setView(ctx->window, ctx->game_view);
    sfRenderWindow_clear(ctx->window, sfBlack);
    if (ctx->game_data.texture_manager)
        draw_textured_floor_and_ceiling(ctx->window,
            ctx->game_data.texture_manager);
    else
        draw_floor_and_celling(ctx->window);
    cast_all_rays(ctx->window, &ctx->game_data);
    if (ctx->game_data.enemies)
        render_all_enemies(ctx->window, ctx->game_data.enemies,
            ctx->game_data.player, &ctx->game_data);
}

void render_game_elements(game_update_context_t *context)
{
    game_context_t *ctx = context->ctx;

    render_game_view(ctx);
    render_weapon(ctx->window, context->weapon);
    if (ctx->light_state) {
        draw_flashlight_overlay(ctx, 500.0f);
        sfRenderWindow_drawSprite(ctx->window, ctx->light_sprite, NULL);
    }
    if (ctx->health_system)
        render_health_sprite(ctx->window, ctx->health_system);
    if (ctx->crosshair)
        draw_crosshair(ctx->crosshair, ctx->window);
    if (ctx->fps_info)
        draw_fps_display(ctx->window, ctx->fps_info);
    sfRenderWindow_display(ctx->window);
}
