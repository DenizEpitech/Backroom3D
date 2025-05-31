/*
** EPITECH PROJECT, 2025
** map
** File description:
** map_structs.h
*/

#ifndef MAP_STRUCTS_H_
    #define MAP_STRUCTS_H_
    #include "wolf.h"
    #include <stdio.h>
    #include <stdlib.h>

typedef struct map_dimensions_s {
    int *width;
    int *height;
} map_dimensions_t;

typedef struct file_reader_s {
    FILE *file;
    char **line;
    size_t *len;
    ssize_t *read;
} file_reader_t;

typedef struct map_parser_context_s {
    map_dimensions_t dimensions;
    file_reader_t reader;
} map_parser_context_t;

typedef struct player_position_s {
    float x;
    float y;
    float angle;
} player_position_t;

typedef struct map_line_context_s {
    char *line;
    game_data_t *game_data;
    int y;
} map_line_context_t;

typedef struct error_context_s {
    int status;
    const char *message;
} error_context_t;

typedef struct memory_allocation_params_s {
    int width;
    int height;
    game_data_t *game_data;
} memory_allocation_params_t;

typedef struct file_loading_params_s {
    const char *filename;
    game_data_t *game_data;
    FILE **file_ptr;
} file_loading_params_t;

typedef struct map_reader_s {
    FILE *file;
    char **line;
    size_t *len;
    ssize_t *read;
    game_data_t *game_data;
} map_reader_t;

typedef struct parser_resources_s {
    FILE *file;
    int *width;
    int *height;
    char **line_ptr;
    size_t *len;
    ssize_t *read;
} parser_resources_t;

#endif
