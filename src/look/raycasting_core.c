/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** raycasting_core
*/

#include "wolf.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static wall_hit_t *init_wall_hit(void)
{
    wall_hit_t *hit = malloc(sizeof(wall_hit_t));

    if (hit == NULL)
        return NULL;
    hit->distance = 0.0f;
    hit->map_x = 0;
    hit->map_y = 0;
    hit->texture_x = 0.0f;
    hit->side = 0;
    hit->map_value = 1;
    return hit;
}

static dda_algo_t *init_dda_algo(game_data_t *game_data, float ray_angle)
{
    dda_algo_t *dda = malloc(sizeof(dda_algo_t));
    float player_x = 0;
    float player_y = 0;

    if (dda == NULL)
        return NULL;
    player_x = game_data->player->x / TILE_SIZE;
    player_y = game_data->player->y / TILE_SIZE;
    dda->ray_angle = ray_angle;
    dda->ray_dir_x = cos(ray_angle);
    dda->ray_dir_y = sin(ray_angle);
    dda->map_x = (int)player_x;
    dda->map_y = (int)player_y;
    dda->pos_x = player_x;
    dda->pos_y = player_y;
    dda->delta_dist_x = fabs(1.0f /
        (dda->ray_dir_x == 0 ? 0.00001f : dda->ray_dir_x));
    dda->delta_dist_y = fabs(1.0f /
        (dda->ray_dir_y == 0 ? 0.00001f : dda->ray_dir_y));
    return dda;
}

static void setup_initial_distances(dda_algo_t *dda, float *side_dist_x,
    float *side_dist_y)
{
    if (dda->ray_dir_x < 0) {
        dda->step_x = -1;
        *side_dist_x = (dda->pos_x - dda->map_x) * dda->delta_dist_x;
    } else {
        dda->step_x = 1;
        *side_dist_x = (dda->map_x + 1.0f - dda->pos_x) * dda->delta_dist_x;
    }
    if (dda->ray_dir_y < 0) {
        dda->step_y = -1;
        *side_dist_y = (dda->pos_y - dda->map_y) * dda->delta_dist_y;
    } else {
        dda->step_y = 1;
        *side_dist_y = (dda->map_y + 1.0f - dda->pos_y) * dda->delta_dist_y;
    }
    return;
}

static int check_ray_bounds(dda_algo_t *dda, game_data_t *game_data)
{
    if (dda->map_x < 0 || dda->map_y < 0 ||
        dda->map_x >= game_data->map_width ||
        dda->map_y >= game_data->map_height)
        return 0;
    if (dda->map_x < 0)
        dda->map_x = 0;
    if (dda->map_y < 0)
        dda->map_y = 0;
    if (dda->map_x >= game_data->map_width)
        dda->map_x = game_data->map_width - 1;
    if (dda->map_y >= game_data->map_height)
        dda->map_y = game_data->map_height - 1;
    return 1;
}

static void perform_dda_step(dda_algo_t *dda, dda_context_t *ctx)
{
    if (ctx->side_dist_x < ctx->side_dist_y) {
        ctx->side_dist_x += dda->delta_dist_x;
        dda->map_x += dda->step_x;
        ctx->side = 0;
    } else {
        ctx->side_dist_y += dda->delta_dist_y;
        dda->map_y += dda->step_y;
        ctx->side = 1;
    }
    return;
}

static int run_dda_algorithm(game_data_t *game_data, dda_algo_t *dda)
{
    dda_context_t ctx = {0.0f, 0.0f, 0, 0, 100};

    setup_initial_distances(dda, &ctx.side_dist_x, &ctx.side_dist_y);
    if (!check_ray_bounds(dda, game_data))
        return 0;
    while (ctx.steps < ctx.max_steps) {
        perform_dda_step(dda, &ctx);
        if (dda->map_x < 0 || dda->map_y < 0 ||
            dda->map_x >= game_data->map_width ||
            dda->map_y >= game_data->map_height)
            break;
        if (game_data->map[dda->map_x][dda->map_y] > 0)
            break;
        ctx.steps++;
    }
    check_ray_bounds(dda, game_data);
    return ctx.side;
}

static void calculate_wall_distance(wall_hit_t *hit, dda_algo_t *dda,
    game_data_t *game_data, int side)
{
    hit_calculation_params_t params = {0};

    if (side == 0) {
        params.distance = (dda->map_x - dda->pos_x +
            (1 - dda->step_x) * 0.5f) / dda->ray_dir_x;
        params.wall_x = dda->pos_y + params.distance * dda->ray_dir_y;
    } else {
        params.distance = (dda->map_y - dda->pos_y +
            (1 - dda->step_y) * 0.5f) / dda->ray_dir_y;
        params.wall_x = dda->pos_x + params.distance * dda->ray_dir_x;
    }
    params.corrected_distance = params.distance *
        cos(dda->ray_angle - game_data->player->angle);
    params.corrected_distance = fmaxf(params.corrected_distance, 0.1f);
    hit->distance = params.corrected_distance * TILE_SIZE;
    return;
}

static void calculate_texture_coordinates(wall_hit_t *hit,
    dda_algo_t *dda, int side)
{
    float wall_x = 0.0f;

    if (side == 0) {
        wall_x = dda->pos_y +
            ((dda->map_x - dda->pos_x +
                (1 - dda->step_x) * 0.5f) / dda->ray_dir_x) *
            dda->ray_dir_y;
    } else {
        wall_x = dda->pos_x +
            ((dda->map_y - dda->pos_y +
                (1 - dda->step_y) * 0.5f) / dda->ray_dir_y) *
            dda->ray_dir_x;
    }
    wall_x -= floor(wall_x);
    hit->texture_x = wall_x;
    if ((side == 0 && dda->ray_dir_x < 0) ||
        (side == 1 && dda->ray_dir_y > 0))
        hit->texture_x = 1.0f - hit->texture_x;
    return;
}

static void set_hit_properties(wall_hit_t *hit, dda_algo_t *dda,
    game_data_t *game_data, int side)
{
    calculate_wall_distance(hit, dda, game_data, side);
    calculate_texture_coordinates(hit, dda, side);
    hit->map_x = dda->map_x;
    hit->map_y = dda->map_y;
    hit->side = side;
    if (dda->map_x >= 0 && dda->map_y >= 0 &&
        dda->map_x < game_data->map_width &&
        dda->map_y < game_data->map_height) {
        hit->map_value = game_data->map[dda->map_x][dda->map_y];
        if (hit->map_value <= 0)
            hit->map_value = 1;
    } else
        hit->map_value = 1;
    return;
}

wall_hit_t *cast_single_ray(game_data_t *game_data, float ray_angle)
{
    wall_hit_t *hit = NULL;
    dda_algo_t *dda = NULL;
    int side = 0;

    if (game_data == NULL || game_data->map == NULL)
        return NULL;
    hit = init_wall_hit();
    dda = init_dda_algo(game_data, ray_angle);
    if (hit == NULL || dda == NULL) {
        if (dda)
            free(dda);
        if (hit)
            free(hit);
        return NULL;
    }
    side = run_dda_algorithm(game_data, dda);
    set_hit_properties(hit, dda, game_data, side);
    free(dda);
    return hit;
}
