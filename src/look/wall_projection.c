/*
** EPITECH PROJECT, 2025
** Wolf3d
** File description:
** wall_projection.c
*/

#include "wolf.h"

sfColor chose_colors(game_data_t *game_data, int map_x, int map_y)
{
    switch (game_data->map[map_x][map_y]) {
        case 1:
            return sfRed;
        case 2:
            return sfGreen;
        case 3:
            return sfBlue;
        case 4:
            return sfWhite;
        default:
            return sfYellow;
    }
}

void render_wall_column(sfRenderWindow *window,
    int column, float distance, sfColor color)
{
    sfVector2u window_size = {0};
    float wall_height = 0.0f;
    float wall_top = 0.0f;
    sfRectangleShape* wall = sfRectangleShape_create();

    window_size = sfRenderWindow_getSize(window);
    distance = fmaxf(distance, 0.1f);
    wall_height = (TILE_SIZE / distance) *
    ((window_size.x / 2) / tan(FOV / 2));
    if (wall_height > window_size.y * 2)
        wall_height = window_size.y * 2;
    wall_top = (window_size.y - wall_height) / 2;
    sfRectangleShape_setSize(wall, (sfVector2f){1, wall_height});
    sfRectangleShape_setPosition(wall, (sfVector2f){column, wall_top});
    sfRectangleShape_setFillColor(wall, color);
    sfRenderWindow_drawRectangleShape(window, wall, NULL);
    sfRectangleShape_destroy(wall);
    return;
}
