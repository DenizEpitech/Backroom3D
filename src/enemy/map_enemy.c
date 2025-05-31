/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_enemy
*/

#include "wolf.h"
#include "enemy.h"
#include <stdlib.h>
#include <stdio.h>

static int is_game_data_valid(game_data_t *game_data)
{
    if (!game_data || !game_data->map)
        return 0;
    return 1;
}

static int is_in_map_bounds(int x, int y, game_data_t *game_data)
{
    if (x < 0 || x >= game_data->map_width)
        return 0;
    if (y < 0 || y >= game_data->map_height)
        return 0;
    return 1;
}

static int is_tile_free(game_data_t *game_data, int map_x, int map_y)
{
    if (!is_in_map_bounds(map_x, map_y, game_data))
        return 0;
    if (game_data->map[map_x][map_y] > 0)
        return 0;
    return 1;
}

static void calculate_corners(float x, float y, int corners[4][2])
{
    float margin = TILE_SIZE * 0.15f;

    corners[0][0] = (int)((x - margin) / TILE_SIZE);
    corners[0][1] = (int)((y - margin) / TILE_SIZE);
    corners[1][0] = (int)((x + margin) / TILE_SIZE);
    corners[1][1] = (int)((y - margin) / TILE_SIZE);
    corners[2][0] = (int)((x - margin) / TILE_SIZE);
    corners[2][1] = (int)((y + margin) / TILE_SIZE);
    corners[3][0] = (int)((x + margin) / TILE_SIZE);
    corners[3][1] = (int)((y + margin) / TILE_SIZE);
}

static int are_corners_free(game_data_t *game_data, int corners[4][2])
{
    int cx = 0;
    int cy = 0;

    for (int i = 0; i < 4; i++) {
        cx = corners[i][0];
        cy = corners[i][1];
        if (!is_in_map_bounds(cx, cy, game_data))
            continue;
        if (game_data->map[cx][cy] > 0)
            return 0;
    }
    return 1;
}

int can_move_to_position(game_data_t *game_data, float x, float y)
{
    int map_x = (int)(x / TILE_SIZE);
    int map_y = (int)(y / TILE_SIZE);
    int corners[4][2] = {0};

    if (!is_game_data_valid(game_data))
        return 0;
    if (!is_tile_free(game_data, map_x, map_y))
        return 0;
    calculate_corners(x, y, corners);
    return are_corners_free(game_data, corners);
}

void place_enemy_from_map_char(game_data_t *game_data,
    int x, int y, char type)
{
    enemy_t *enemy = NULL;
    float pos_x = 0.0f;
    float pos_y = 0.0f;
    enemy_type_t enemy_type = {0};

    pos_x = (x + 0.5f) * TILE_SIZE;
    pos_y = (y + 0.5f) * TILE_SIZE;
    switch (type) {
        case 'E':
            enemy_type = ENEMY_TYPE_SOLDIER;
            break;
    }
    enemy = create_enemy_with_type(pos_x, pos_y, enemy_type);
    if (!enemy)
        return;
    enemy->data = game_data;
    add_enemy_to_list(&game_data->enemies, enemy);
    game_data->enemy_count++;
    game_data->map[x][y] = 0;
}

static void check_and_place_enemy(game_data_t *game_data, int x, int y)
{
    if (game_data->map[x][y] == 'E') {
        place_enemy_from_map_char(game_data, x, y,
            (char)game_data->map[x][y]);
    }
}

static void scan_enemies_in_map(game_data_t *game_data)
{
    int x = 0;
    int y = 0;

    for (x = 0; x < game_data->map_width; x++) {
        for (y = 0; y < game_data->map_height; y++) {
            check_and_place_enemy(game_data, x, y);
        }
    }
}

void scan_map_for_enemies(game_data_t *game_data)
{
    if (!game_data || !game_data->map)
        return;
    game_data->enemy_count = 0;
    game_data->enemies = NULL;
    scan_enemies_in_map(game_data);
}
