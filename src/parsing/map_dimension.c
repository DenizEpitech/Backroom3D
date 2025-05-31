/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_dimension
*/

#include "wolf.h"
#include "map_struct.h"
#include "map_processing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int process_dimension_header(char *line, int *width, int *height)
{
    if (!line || !width || !height)
        return EXIT_ERROR;
    if (strncmp(line, "#width:", 7) == 0) {
        *width = atoi(line + 7);
        return (*width <= 0 || *width > 1000) ? EXIT_ERROR : FALSE;
    }
    if (strncmp(line, "#height:", 8) == 0) {
        *height = atoi(line + 8);
        return (*height <= 0 || *height > 1000) ? EXIT_ERROR : FALSE;
    }
    return EXIT_SUCCESS;
}

int check_if_map_data(char *line)
{
    if (!line)
        return EXIT_ERROR;
    if (line[0] != '#')
        return TRUE;
    return EXIT_SUCCESS;
}

static int process_map_header_line(char *line, int *width, int *height)
{
    int result = 0;

    if (!line || !width || !height)
        return EXIT_ERROR;
    result = process_dimension_header(line, width, height);
    if (result != EXIT_SUCCESS)
        return result;
    return check_if_map_data(line);
}

static int process_dimension_line_single(parser_resources_t *res,
    int *found_data)
{
    int result = 0;

    if (!res || !found_data || !res->line_ptr || !*res->line_ptr)
        return EXIT_ERROR;
    result = process_map_header_line(*res->line_ptr,
        res->width, res->height);
    if (result == EXIT_ERROR) {
        free(*res->line_ptr);
        *res->line_ptr = NULL;
        return EXIT_ERROR;
    }
    if (result == TRUE) {
        fseek(res->file, -(*res->read), SEEK_CUR);
        *found_data = TRUE;
        return TRUE;
    }
    *res->read = getline(res->line_ptr, res->len, res->file);
    return EXIT_SUCCESS;
}

static int read_dimension_lines(parser_resources_t *res)
{
    int found_data = 0;
    int status = 0;

    if (!res || !res->file || !res->width || !res->height || !res->line_ptr)
        return EXIT_ERROR;
    while (*res->read != FUNCTION_ERROR) {
        status = process_dimension_line_single(res, &found_data);
        if (status == EXIT_ERROR)
            return EXIT_ERROR;
        if (status == TRUE)
            break;
    }
    return found_data;
}

static int setup_parse_resources(FILE *file, int *width,
    int *height, parser_resources_t *res)
{
    char **line_ptr = malloc(sizeof(char *));

    if (!file || !width || !height || !res || !line_ptr)
        return EXIT_ERROR;
    *line_ptr = NULL;
    res->file = file;
    res->width = width;
    res->height = height;
    res->line_ptr = line_ptr;
    if (allocate_reader_resources(res) != EXIT_SUCCESS) {
        free(line_ptr);
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

static int read_first_header_line(parser_resources_t *res)
{
    if (!res || !res->file || !res->line_ptr || !res->len || !res->read)
        return EXIT_ERROR;
    *res->read = getline(res->line_ptr, res->len, res->file);
    if (*res->read == FUNCTION_ERROR) {
        if (*res->line_ptr)
            free(*res->line_ptr);
        free(res->line_ptr);
        free(res->len);
        free(res->read);
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

static int init_parse_resources(FILE *file, int *width,
    int *height, parser_resources_t *res)
{
    int status = 0;

    status = setup_parse_resources(file, width, height, res);
    if (status != EXIT_SUCCESS)
        return EXIT_ERROR;
    status = read_first_header_line(res);
    if (status != EXIT_SUCCESS)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

int parse_map_dimensions(FILE *file, int *width, int *height)
{
    parser_resources_t res = {0};
    int found_data = 0;
    int status = 0;

    if (!file || !width || !height)
        return EXIT_ERROR;
    status = init_parse_resources(file, width, height, &res);
    if (status != EXIT_SUCCESS)
        return EXIT_ERROR;
    found_data = read_dimension_lines(&res);
    if (*res.line_ptr)
        free(*res.line_ptr);
    free(res.line_ptr);
    free(res.len);
    free(res.read);
    return found_data;
}
