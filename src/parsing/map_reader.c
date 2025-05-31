/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_reader
*/

#include "wolf.h"
#include "map_struct.h"
#include "map_processing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int read_next_map_line(map_reader_t *reader)
{
    char **line = reader->line;

    if (!reader->file || !line || !reader->len || !reader->read)
        return EXIT_ERROR;
    *reader->read = getline(line, reader->len, reader->file);
    if (*reader->read == FUNCTION_ERROR || !*line)
        return FALSE;
    while ((*line)[0] == '#' || *reader->read <= 1) {
        free(*line);
        *line = NULL;
        *reader->read = getline(line, reader->len, reader->file);
        if (*reader->read == FUNCTION_ERROR || !*line)
            return FALSE;
    }
    return TRUE;
}

static int process_map_lines_loop(map_reader_t *reader,
    game_data_t *game_data, int *y)
{
    int status = 0;

    if (!reader || !game_data || !y)
        return EXIT_ERROR;
    while (*y < game_data->map_height) {
        status = read_next_map_line(reader);
        if (status != TRUE)
            break;
        process_map_line(game_data, *(reader->line), (*y)++);
        free(*(reader->line));
        *(reader->line) = NULL;
    }
    return EXIT_SUCCESS;
}

int read_map_lines(FILE *file, game_data_t *game_data, int *lines_read)
{
    map_reader_t reader = {0};
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    int y = 0;
    int status = 0;

    if (!file || !game_data || !lines_read)
        return EXIT_ERROR;
    reader.file = file;
    reader.line = &line;
    reader.len = &len;
    reader.read = &read;
    reader.game_data = game_data;
    status = process_map_lines_loop(&reader, game_data, &y);
    if (status != EXIT_SUCCESS)
        return status;
    *lines_read = y;
    return EXIT_SUCCESS;
}

int read_map_data(FILE *file, game_data_t *game_data, player_t *player)
{
    int lines_read = 0;
    int status = 0;

    if (!file || !game_data || !player)
        return EXIT_ERROR;
    game_data->player = player;
    status = read_map_lines(file, game_data, &lines_read);
    if (status != EXIT_SUCCESS)
        return status;
    status = finalize_map_data(game_data, lines_read);
    if (status != EXIT_SUCCESS)
        return status;
    return EXIT_SUCCESS;
}
