/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_render
*/

#include "enemy.h"
#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int compare_enemies_by_distance(const void *a, const void *b)
{
    enemy_render_data_t *enemy_a = (enemy_render_data_t *)a;
    enemy_render_data_t *enemy_b = (enemy_render_data_t *)b;

    if (enemy_a->distance > enemy_b->distance)
        return -1;
    if (enemy_a->distance < enemy_b->distance)
        return 1;
    return 0;
}

void render_enemy_with_data(sfRenderWindow *window,
    enemy_render_data_t *data)
{
    int current_frame = 0;
    sfSprite *sprite = NULL;
    sfVector2u tex_size = {0};

    if (!data->visible || !data->enemy->animations.current)
        return;
    if (data->enemy->state == ENEMY_STATE_DYING) {
        if (data->enemy->animations.current->current_frame >=
            data->enemy->animations.current->frame_count - 1)
            return;
    }
    current_frame = data->enemy->animations.current->current_frame;
    sprite = data->enemy->animations.current->frames[current_frame].sprite;
    sfSprite_setPosition(sprite, (sfVector2f){data->screen_x,
        sfRenderWindow_getSize(window).y / 2});
    tex_size = sfTexture_getSize(
        data->enemy->animations.current->frames[current_frame].texture);
    sfSprite_setScale(sprite, (sfVector2f)
    {data->size / tex_size.x, data->size / tex_size.y});
    sfRenderWindow_drawSprite(window, sprite, NULL);
}

static int count_enemies(enemy_t *enemy_list)
{
    int count = 0;
    enemy_t *current = enemy_list;

    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

static enemy_render_data_t *allocate_render_data(int count)
{
    if (count == 0)
        return NULL;
    return malloc(count * sizeof(enemy_render_data_t));
}

static void calculate_render_data(enemy_render_data_t *data,
    enemy_t *enemy, player_t *player)
{
    data->enemy = enemy;
    data->distance = calculate_enemy_distance(enemy, player);
    data->angle = calculate_enemy_angle(enemy, player);
}

static void set_display_params(enemy_render_data_t *data,
    sfVector2u window_size)
{
    data->screen_x = (0.5f + data->angle / FOV) * window_size.x;
    data->size = TILE_SIZE / data->distance *
        ((window_size.x / 2) / tanf(FOV / 2));
}

static void fill_render_data(enemy_render_data_t *render_data,
    enemy_t *enemy_list, player_t *player, game_data_t *game_data)
{
    enemy_t *current = enemy_list;
    int i = 0;

    while (current) {
        calculate_render_data(&render_data[i], current, player);
        render_data[i].visible = is_enemy_visible(current, player, game_data);
        current->direction = calculate_enemy_direction(current, player);
        i++;
        current = current->next;
    }
}

static void configure_display_all(enemy_render_data_t *render_data,
    int count, sfRenderWindow *window)
{
    sfVector2u window_size = sfRenderWindow_getSize(window);

    for (int i = 0; i < count; i++) {
        set_display_params(&render_data[i], window_size);
    }
}

static void render_sorted_enemies(sfRenderWindow *window,
    enemy_render_data_t *render_data, int count)
{
    qsort(render_data, count, sizeof(enemy_render_data_t),
        compare_enemies_by_distance);
    for (int i = 0; i < count; i++) {
        render_enemy_with_data(window, &render_data[i]);
    }
}

void render_all_enemies(sfRenderWindow *window, enemy_t *enemy_list,
    player_t *player, game_data_t *game_data)
{
    int count = count_enemies(enemy_list);
    enemy_render_data_t *render_data = NULL;

    render_data = allocate_render_data(count);
    if (!render_data)
        return;
    fill_render_data(render_data, enemy_list, player, game_data);
    configure_display_all(render_data, count, window);
    render_sorted_enemies(window, render_data, count);
    free(render_data);
}
