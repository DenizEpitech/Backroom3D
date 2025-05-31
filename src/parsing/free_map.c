/*
** EPITECH PROJECT, 2025
** map
** File description:
** free_map.c
*/

#include "wolf.h"
#include <stdlib.h>

void free_map_rows(int **map, int width)
{
    if (map == NULL)
        return;
    for (int i = 0; i < width; i++) {
        if (map[i])
            free(map[i]);
    }
    return;
}

void free_game_data(game_data_t *game_data)
{
    if (game_data->map) {
        free_map_rows(game_data->map, game_data->map_width);
        free(game_data->map);
        game_data->map = NULL;
    }
    if (game_data->texture_manager) {
        destroy_texture_manager(game_data->texture_manager);
        game_data->texture_manager = NULL;
    }
    if (game_data->enemies) {
        destroy_all_enemies(game_data->enemies);
        game_data->enemies = NULL;
        game_data->enemy_count = 0;
    }
    return;
}

void cleanup_partial_allocation(int **map, int allocated_rows)
{
    if (map == NULL)
        return;
    for (int j = 0; j < allocated_rows; j++) {
        if (map[j])
            free(map[j]);
    }
    free(map);
    return;
}
