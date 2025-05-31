/*
** EPITECH PROJECT, 2025
** Wolf3d
** File description:
** initialize_engine.c
*/

#include "wolf.h"

shoot_effect_t init_shoot_effect(void)
{
    shoot_effect_t effect = {0};

    effect.alpha = 255.0f;
    effect.cooldown = 0.0f;
    effect.sound = NULL;
    effect.buffer = NULL;
    effect.position = (sfVector2i) {WIDTH / 2, HEIGHT / 2};
    return effect;
}

void init_player(player_t *player)
{
    if (player == NULL)
        return;
    player->x = TILE_SIZE * 1.5f;
    player->y = TILE_SIZE * 1.5f;
    player->angle = 0.0f;
    player->data = NULL;
    player->context = NULL;
    player->is_shooting = 0;
    player->shoot_timer = 0.0f;
    player->shoot.alpha = 0.0f;
    player->shoot.position = (sfVector2i){0, 0};
    return;
}

int is_wall(game_data_t *game_data, int x, int y)
{
    int map_x = x / TILE_SIZE;
    int map_y = y / TILE_SIZE;

    if (!game_data || !game_data->map)
        return TRUE;
    if (map_x < 0 || map_y < 0 ||
        map_x >= game_data->map_width ||
        map_y >= game_data->map_height ||
        game_data->map[map_x][map_y] > 0)
        return TRUE;
    return FALSE;
}

void draw_floor_and_celling(sfRenderWindow *window)
{
    sfVector2u win_size = sfRenderWindow_getSize(window);
    sfRectangleShape* floor = sfRectangleShape_create();
    sfRectangleShape* roof = sfRectangleShape_create();
    sfColor floor_color = {0};
    sfColor roof_color = {0};

    sfRectangleShape_setSize(floor, (sfVector2f){win_size.x, win_size.y / 2});
    sfRectangleShape_setPosition(floor, (sfVector2f){0, win_size.y / 2});
    floor_color = sfColor_fromRGB(56, 161, 56);
    sfRectangleShape_setFillColor(floor, floor_color);
    sfRenderWindow_drawRectangleShape(window, floor, NULL);
    sfRectangleShape_setSize(roof, (sfVector2f){win_size.x, win_size.y / 2});
    roof_color = sfColor_fromRGB(76, 168, 224);
    sfRectangleShape_setFillColor(roof, roof_color);
    sfRenderWindow_drawRectangleShape(window, roof, NULL);
    sfRectangleShape_destroy(floor);
    sfRectangleShape_destroy(roof);
    return;
}
