/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_character
*/

#include "wolf.h"
#include "map_struct.h"
#include "map_processing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void process_map_characters(game_data_t *game_data,
    char *line, int y, int line_length)
{
    int x = 0;

    if (!game_data || !line || y < 0 || y >= game_data->map_height)
        return;
    for (x = 0; x < game_data->map_width && x < line_length; x++) {
        if (line[x] >= '0' && line[x] <= '9')
            game_data->map[x][y] = line[x] - '0';
        if (line[x] == 'P' && game_data->player) {
            game_data->player->x = TILE_SIZE * (x + 0.5f);
            game_data->player->y = TILE_SIZE * (y + 0.5f);
            game_data->map[x][y] = 0;
        }
        if (line[x] == 'E')
            game_data->map[x][y] = line[x];
    }
    return;
}

void fill_horizontal_line(game_data_t *game_data, int y)
{
    int x = 0;

    if (!game_data || y < 0 || y >= game_data->map_height)
        return;
    for (x = 0; x < game_data->map_width; x++) {
        if (game_data->map[x][y] == 0)
            game_data->map[x][y] = 1;
    }
    return;
}

void ensure_map_boundaries(game_data_t *game_data, int y)
{
    if (!game_data || y < 0 || y >= game_data->map_height)
        return;
    if (y == 0 || y == game_data->map_height - 1)
        fill_horizontal_line(game_data, y);
    game_data->map[0][y] = 1;
    game_data->map[game_data->map_width - 1][y] = 1;
    return;
}

void process_map_line(game_data_t *game_data, char *line, int y)
{
    int line_length = 0;
    char *newline = NULL;

    if (!game_data || !line || y < 0 || y >= game_data->map_height)
        return;
    newline = strchr(line, '\n');
    if (newline)
        *newline = '\0';
    line_length = strlen(line);
    process_map_characters(game_data, line, y, line_length);
    ensure_map_boundaries(game_data, y);
    return;
}

int finalize_map_data(game_data_t *game_data, int lines_read)
{
    int x = 0;

    if (game_data == NULL)
        return EXIT_ERROR;
    for (int i = lines_read; i < game_data->map_height; i++) {
        for (x = 0; x < game_data->map_width; x++)
            game_data->map[x][i] = 1;
    }
    if (game_data->player->x <= 0 || game_data->player->y <= 0) {
        game_data->player->x = TILE_SIZE * 1.5f;
        game_data->player->y = TILE_SIZE * 1.5f;
        game_data->player->angle = 0;
    }
    return EXIT_SUCCESS;
}
