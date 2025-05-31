/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wall_renderer.c - rendu des murs texturés
*/

#include "wolf.h"
#include "texture_renderer.h"
#include <stdio.h>
#include <stdlib.h>

static wall_render_params_t init_wall_render_params(sfRenderWindow *window,
    wall_hit_t *hit, texture_manager_t *texture_manager)
{
    wall_render_params_t params = {0};

    params.window_size = sfRenderWindow_getSize(window);
    params.wall = sfRectangleShape_create();
    hit->distance = fmaxf(hit->distance, 0.1f);
    params.height = (TILE_SIZE / hit->distance) *
        ((params.window_size.x / 2) / tan(FOV / 2));
    if (params.height > params.window_size.y * 2)
        params.height = params.window_size.y * 2;
    params.top = (params.window_size.y - params.height) / 2;
    params.texture_index = hit->map_value - 1;
    if (params.texture_index < 0)
        params.texture_index = 0;
    if (params.texture_index >= MAX_TEXTURES)
        params.texture_index = 0;
    if (!texture_manager->wall_textures[params.texture_index])
        params.texture_index = 0;
    return params;
}

static void apply_wall_texture(wall_render_params_t *params,
    wall_hit_t *hit, texture_manager_t *texture_manager)
{
    sfVector2u tex_size = {0};
    int tex_x = 0;
    sfIntRect texture_rect = {0};

    if (!params || !hit || !texture_manager)
        return;
    if (!texture_manager->wall_textures[params->texture_index])
        return;
    tex_size = sfTexture_getSize
    (texture_manager->wall_textures[params->texture_index]);
    tex_x = (int)(hit->texture_x * tex_size.x);
    if (tex_x < 0)
        tex_x = 0;
    if ((unsigned int)tex_x >= tex_size.x)
        tex_x = tex_size.x - 1;
    texture_rect = (sfIntRect){tex_x, 0, 1, tex_size.y};
    sfRectangleShape_setTexture(params->wall,
        texture_manager->wall_textures[params->texture_index], sfTrue);
    sfRectangleShape_setTextureRect(params->wall, texture_rect);
    return;
}

static void apply_wall_color(wall_render_params_t *params,
    wall_hit_t *hit)
{
    sfColor color = {0};

    if (!params || !hit)
        return;
    color = (hit->side == 0) ?
    sfColor_fromRGB(200, 0, 0) : sfColor_fromRGB(150, 0, 0);
    sfRectangleShape_setFillColor(params->wall, color);
    return;
}

static void apply_texture_to_wall(wall_render_params_t *params,
    wall_hit_t *hit, texture_manager_t *texture_manager)
{
    if (!params || !hit || !texture_manager)
        return;
    if (texture_manager->wall_textures[params->texture_index])
        apply_wall_texture(params, hit, texture_manager);
    else
        apply_wall_color(params, hit);
    return;
}

void render_textured_wall_column(sfRenderWindow *window, int column,
    wall_hit_t *hit, texture_manager_t *texture_manager)
{
    wall_render_params_t params = {0};

    if (!window || !hit || !texture_manager)
        return;
    params = init_wall_render_params(window, hit, texture_manager);
    if (!params.wall)
        return;
    sfRectangleShape_setSize(params.wall, (sfVector2f){1, params.height});
    sfRectangleShape_setPosition(params.wall,
        (sfVector2f){column, params.top});
    apply_texture_to_wall(&params, hit, texture_manager);
    sfRenderWindow_drawRectangleShape(window, params.wall, NULL);
    sfRectangleShape_destroy(params.wall);
    return;
}
