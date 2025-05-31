/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game_core
*/

#include "wolf.h"
#include "weapon.h"
#include <stdlib.h>
#include <stdio.h>

int run_game_loop(game_context_t *ctx, player_t *player)
{
    game_loop_resources_t resources = {0};

    if (init_game_loop_resources(&resources, ctx, player) != EXIT_SUCCESS)
        return EXIT_ERROR;
    while (sfRenderWindow_isOpen(ctx->window) && *ctx->state == GAME) {
        sfRenderWindow_setFramerateLimit(ctx->window, 120);
        sfRenderWindow_setView(ctx->window, ctx->game_view);
        process_game_frame(&resources);
        if (ctx->game_data.enemy_count <= 0 &&
            ctx->game_data.enemies == NULL) {
            cleanup_game_loop_ressources(&resources);
            return GAME_NEXT_LEVEL;
        }
    }
    sfRenderWindow_setView(ctx->window,
        sfRenderWindow_getDefaultView(ctx->window));
    cleanup_game_loop_ressources(&resources);
    cleanup_game_components(ctx);
    return (*ctx->state == MENU) ? EXIT_SUCCESS : GAME_EXIT;
}

static init_context_params_t setup_init_params(sfRenderWindow *window,
    game_state_t *state, fps_info_t *fps_info, char *map_path)
{
    init_context_params_t params = {0};

    params.window = window;
    params.state = state;
    params.fps_info = fps_info;
    params.map_path = map_path;
    return params;
}

static game_context_t init_shoot_context(sfRenderWindow *window)
{
    sfSound *shoot_sound = NULL;
    sfSoundBuffer *shoot_buffer = NULL;
    game_context_t context = {0};

    init_shoot_sound(&shoot_sound, &shoot_buffer);
    context.window = window;
    context.shoot_sound = shoot_sound;
    context.shoot_buffer = shoot_buffer;
    return context;
}

static int initialize_game_context_base(game_context_t *ctx,
    init_context_params_t params)
{
    game_context_t sound_ctx = {0};

    if (!params.window || !params.state)
        return EXIT_ERROR;
    ctx->window = params.window;
    ctx->state = params.state;
    ctx->fps_info = params.fps_info;
    ctx->map_path = params.map_path;
    ctx->event = (sfEvent){0};
    ctx->game_data = (game_data_t){0};
    ctx->game_data.texture_manager = NULL;
    ctx->game_data.player = params.player;
    ctx->game_data.map = NULL;
    ctx->crosshair = NULL;
    sound_ctx = init_shoot_context(ctx->window);
    ctx->shoot_sound = sound_ctx.shoot_sound;
    ctx->shoot_buffer = sound_ctx.shoot_buffer;
    init_game_view(ctx, params);
    return initialise_lightning(ctx);
}

static int initialize_game_resources(game_context_t *ctx,
    player_t *player)
{
    ctx->game_data.player = player;
    if (load_map_from_file(ctx->map_path,
        &ctx->game_data, player)
        != EXIT_SUCCESS)
        return EXIT_ERROR;
    ctx->game_data.texture_manager = init_texture_manager();
    return EXIT_SUCCESS;
}

static int initialize_game_context(game_context_t *ctx,
    init_context_params_t params)
{
    int status = 0;

    status = initialize_game_context_base(ctx, params);
    if (status != EXIT_SUCCESS)
        return EXIT_ERROR;
    status = initialize_game_resources(ctx, params.player);
    if (status != EXIT_SUCCESS)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

static int setup_player(player_t *player, game_context_t *ctx)
{
    init_player(player);
    ctx->game_data.player = player;
    player->context = ctx;
    player->data = &ctx->game_data;
    return EXIT_SUCCESS;
}

static void cleanup_level_resources(level_ctx_t *level)
{
    if (level->ctx->health_system)
        destroy_health_sprite_system(level->ctx->health_system);
    cleanup_game_components(level->ctx);
    free_game_data(&level->ctx->game_data);
    if (*level->state == GAME)
        display_completed_level(level->window);
}

static int process_level(level_ctx_t *level)
{
    init_context_params_t params = setup_init_params(level->window,
        level->state, level->fps_info, (char *)level->map_path);
    int status = 0;

    params.player = level->player;
    if (initialize_game_context(level->ctx, params) != EXIT_SUCCESS ||
        setup_player(level->player, level->ctx) != EXIT_SUCCESS ||
        setup_health_system(level->ctx) != EXIT_SUCCESS)
        return EXIT_ERROR;
    init_game_components(level->ctx);
    status = run_game_loop(level->ctx, level->player);
    if (status == EXIT_ERROR || save_file(level->ctx) == EXIT_ERROR) {
        cleanup_game_components(level->ctx);
        return EXIT_ERROR;
    }
    cleanup_level_resources(level);
    return status;
}

int start_game(sfRenderWindow *window, game_state_t *state,
    fps_info_t *fps_info)
{
    const char *map_paths[] = {"map/floor", "map/ground", "map/underground",
        NULL};
    game_context_t ctx = {0};
    level_ctx_t level = init_level(window, state, fps_info, &ctx);
    player_t player = {0};
    int status = 0;

    for (int i = 0; map_paths[i] != NULL; i++) {
        level.map_path = map_paths[i];
        level.player = &player;
        status = process_level(&level);
        if (status == EXIT_ERROR)
            return EXIT_ERROR;
        if (status == GAME_EXIT)
            break;
        if (status == GAME_NEXT_LEVEL && map_paths[i + 1] == NULL)
            manage_game_state(window, state);
    }
    return EXIT_SUCCESS;
}
