/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_loader
*/

#include "wolf.h"
#include "map_struct.h"
#include "map_processing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int init_map_dimensions(const char *filename,
    game_data_t *game_data, FILE **file_ptr)
{
    FILE *file = NULL;

    if (!filename || !game_data || !file_ptr)
        return EXIT_ERROR;
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open map file %s\n", filename);
        return EXIT_ERROR;
    }
    if (parse_map_dimensions(file, &game_data->map_width,
        &game_data->map_height) != TRUE) {
        fprintf(stderr, "Error: Failed to parse map dimensions\n");
        fclose(file);
        return EXIT_ERROR;
    }
    *file_ptr = file;
    return EXIT_SUCCESS;
}

int load_map_content(FILE *file, game_data_t *game_data, player_t *player)
{
    int status = 0;

    if (!file || !game_data || !player)
        return EXIT_ERROR;
    if (allocate_map_memory(game_data) != EXIT_SUCCESS)
        return EXIT_ERROR;
    status = read_map_data(file, game_data, player);
    if (status != EXIT_SUCCESS) {
        free_game_data(game_data);
        return EXIT_ERROR;
    }
    scan_map_for_enemies(game_data);
    return EXIT_SUCCESS;
}

int load_map_from_file(const char *filename, game_data_t *game_data,
    player_t *player)
{
    FILE *file = NULL;
    int status = 0;

    if (!filename || !game_data || !player)
        return EXIT_ERROR;
    status = init_map_dimensions(filename, game_data, &file);
    if (status != EXIT_SUCCESS)
        return EXIT_ERROR;
    status = load_map_content(file, game_data, player);
    if (status != EXIT_SUCCESS) {
        fclose(file);
        return EXIT_ERROR;
    }
    fclose(file);
    return EXIT_SUCCESS;
}
