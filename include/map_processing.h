/*
** EPITECH PROJECT, 2025
** wold3d
** File description:
** map_processing
*/

#ifndef MAP_PROCESSING_H_
    #define MAP_PROCESSING_H_
    #include "wolf.h"
    #include "map_struct.h"
    #include <stdio.h>

typedef struct floor_ceiling_shapes_s {
    sfRectangleShape* floor;
    sfRectangleShape* ceiling;
    sfVector2u win_size;
} floor_ceiling_shapes_t;

int load_map_from_file(const char *filename, game_data_t *game_data,
    player_t *player);
void fill_horizontal_line(game_data_t *game_data, int y);
void ensure_map_boundaries(game_data_t *game_data, int y);
void process_map_line(game_data_t *game_data, char *line, int y);

int process_dimension_header(char *line, int *width, int *height);
int check_if_map_data(char *line);
int parse_map_dimensions(FILE *file, int *width, int *height);

int allocate_map_memory(game_data_t *game_data);
int allocate_reader_resources(parser_resources_t *res);
void cleanup_partial_allocation(int **map, int allocated_rows);

int read_next_map_line(map_reader_t *reader);
int read_map_lines(FILE *file, game_data_t *game_data, int *lines_read);
int read_map_data(FILE *file, game_data_t *game_data, player_t *player);

int init_map_dimensions(const char *filename, game_data_t *game_data,
    FILE **file_ptr);
int load_map_content(FILE *file, game_data_t *game_data, player_t *player);
void process_map_characters(game_data_t *game_data, char *line,
    int y, int line_length);
int finalize_map_data(game_data_t *game_data, int lines_read);

#endif
