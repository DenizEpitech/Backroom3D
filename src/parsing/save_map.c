/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d
** File description:
** save_map.c
*/

#include "wolf.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static void write_in_file(game_context_t *ctx, int fd, int i)
{
    for (int j = 0; j < ctx->game_data.map_width; j++) {
        if (j == ctx->game_data.map_width - 1)
            dprintf(fd, "%d\n", ctx->game_data.map[i][j]);
        else
            dprintf(fd, "%d", ctx->game_data.map[i][j]);
    }
    return;
}

int save_file(game_context_t *ctx)
{
    char filename[50] = {0};
    int fd = 0;

    snprintf(filename, sizeof(filename), "%s.save", ctx->map_path);
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
        return EXIT_ERROR;
    dprintf(fd, "#width: %d\n", ctx->game_data.map_width);
    dprintf(fd, "#height: %d\n", ctx->game_data.map_height);
    for (int i = 0; i < ctx->game_data.map_height; i++)
        write_in_file(ctx, fd, i);
    close(fd);
    return EXIT_SUCCESS;
}
