/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** floor_ceiling_rendering
*/

#include "wolf.h"
#include "texture_renderer.h"
#include <stdio.h>
#include <stdlib.h>

static floor_ceiling_shapes_t init_floor_ceiling_shapes(sfRenderWindow *window)
{
    floor_ceiling_shapes_t shapes = {0};

    shapes.win_size = sfRenderWindow_getSize(window);
    shapes.floor = sfRectangleShape_create();
    shapes.ceiling = sfRectangleShape_create();
    if (shapes.floor) {
        sfRectangleShape_setSize(shapes.floor,
            (sfVector2f){shapes.win_size.x, shapes.win_size.y / 2});
        sfRectangleShape_setPosition(shapes.floor,
            (sfVector2f){0, shapes.win_size.y / 2});
    }
    if (shapes.ceiling) {
        sfRectangleShape_setSize(shapes.ceiling,
            (sfVector2f){shapes.win_size.x, shapes.win_size.y / 2});
        sfRectangleShape_setPosition(shapes.ceiling, (sfVector2f){0, 0});
    }
    return shapes;
}

static void apply_floor_texture(floor_ceiling_shapes_t *shapes,
    texture_manager_t *texture_manager)
{
    sfIntRect floor_rect = {0};

    if (!shapes->floor || !texture_manager)
        return;
    if (texture_manager->floor_texture) {
        sfTexture_setRepeated(texture_manager->floor_texture, sfTrue);
        floor_rect = (sfIntRect){0, 0,
            shapes->win_size.x / 4, shapes->win_size.y / 4
        };
        sfRectangleShape_setTexture(shapes->floor,
            texture_manager->floor_texture, sfTrue);
        sfRectangleShape_setTextureRect(shapes->floor, floor_rect);
    } else {
        sfRectangleShape_setFillColor(shapes->floor,
            sfColor_fromRGB(56, 161, 56));
    }
    return;
}

static void apply_ceiling_texture(floor_ceiling_shapes_t *shapes,
    texture_manager_t *texture_manager)
{
    sfIntRect ceiling_rect = {0};

    if (!shapes->ceiling || !texture_manager)
        return;
    if (texture_manager->ceiling_texture) {
        sfTexture_setRepeated(texture_manager->ceiling_texture, sfTrue);
        ceiling_rect = (sfIntRect){0, 0,
            shapes->win_size.x, shapes->win_size.y / 2
        };
        sfRectangleShape_setTexture(shapes->ceiling,
            texture_manager->ceiling_texture, sfTrue);
        sfRectangleShape_setTextureRect(shapes->ceiling, ceiling_rect);
    } else {
        sfRectangleShape_setFillColor(shapes->ceiling,
            sfColor_fromRGB(76, 168, 224));
    }
    return;
}

void draw_textured_floor_and_ceiling(sfRenderWindow *window,
    texture_manager_t *texture_manager)
{
    floor_ceiling_shapes_t shapes = {0};

    if (!window || !texture_manager)
        return;
    shapes = init_floor_ceiling_shapes(window);
    if (!shapes.floor || !shapes.ceiling) {
        if (shapes.floor)
            sfRectangleShape_destroy(shapes.floor);
        if (shapes.ceiling)
            sfRectangleShape_destroy(shapes.ceiling);
        return;
    }
    apply_floor_texture(&shapes, texture_manager);
    apply_ceiling_texture(&shapes, texture_manager);
    sfRenderWindow_drawRectangleShape(window, shapes.floor, NULL);
    sfRenderWindow_drawRectangleShape(window, shapes.ceiling, NULL);
    sfRectangleShape_destroy(shapes.floor);
    sfRectangleShape_destroy(shapes.ceiling);
    return;
}
