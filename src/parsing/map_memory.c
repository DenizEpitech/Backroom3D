/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_memory
*/

#include "wolf.h"
#include "map_struct.h"
#include "map_processing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int allocate_reader_resources(parser_resources_t *res)
{
    if (!res)
        return EXIT_ERROR;
    res->len = malloc(sizeof(size_t));
    res->read = malloc(sizeof(ssize_t));
    if (!res->len || !res->read) {
        free(res->len);
        free(res->read);
        return EXIT_ERROR;
    }
    *res->len = 0;
    *res->read = 0;
    return EXIT_SUCCESS;
}

int allocate_map_memory(game_data_t *game_data)
{
    int i = 0;

    if (!game_data || game_data->map_width <= 0 || game_data->map_height <= 0)
        return EXIT_ERROR;
    game_data->map = (int **)malloc(game_data->map_width * sizeof(int *));
    if (!game_data->map)
        return EXIT_ERROR;
    for (i = 0; i < game_data->map_width; i++) {
        game_data->map[i] = (int *)malloc(game_data->map_height * sizeof(int));
        if (!game_data->map[i]) {
            cleanup_partial_allocation(game_data->map, i);
            game_data->map = NULL;
            return EXIT_ERROR;
        }
        memset(game_data->map[i], 0, game_data->map_height * sizeof(int));
    }
    return EXIT_SUCCESS;
}
