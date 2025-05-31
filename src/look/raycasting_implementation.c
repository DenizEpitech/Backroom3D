/*
** EPITECH PROJECT, 2025
** Wolf3d
** File description:
** raycasting_implementation.c
*/

#include "wolf.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static void process_ray(sfRenderWindow *window, game_data_t *game_data,
    unsigned int column, float ray_angle)
{
    wall_hit_t *hit = cast_single_ray(game_data, ray_angle);
    sfColor color = {0};

    if (hit == NULL)
        return;
    if (game_data->texture_manager)
        render_textured_wall_column(window, column, hit,
            game_data->texture_manager);
    else {
        color = chose_colors(game_data, hit->map_x, hit->map_y);
        render_wall_column(window, column, hit->distance, color);
    }
    free(hit);
    return;
}

void cast_all_rays(sfRenderWindow *window, game_data_t *game_data)
{
    sfVector2u win_size = {0};
    unsigned int screen_width = 0;
    float ray_angle = 0.0f;
    float angle_step = 0.0f;
    unsigned int i = 0;

    if (!window || !game_data || !game_data->player || !game_data->map) {
        return;
    }
    win_size = sfRenderWindow_getSize(window);
    screen_width = win_size.x;
    ray_angle = game_data->player->angle - (FOV / 2);
    angle_step = FOV / screen_width;
    for (i = 0; i < screen_width; i++) {
        process_ray(window, game_data, i, ray_angle);
        ray_angle += angle_step;
    }
    return;
}
