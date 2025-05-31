/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d
** File description:
** crosshair_animation.c
*/

#include "wolf.h"
#include <SFML/Graphics.h>
#include <stdlib.h>
#include "weapon.h"

sfVector2i get_crosshair_position(sfRenderWindow *window)
{
    sfVector2u win_size = sfRenderWindow_getSize(window);
    sfVector2i crosshair = {win_size.x / 2, win_size.y / 2};

    return crosshair;
}

static crosshair_t *allocate_crosshair(void)
{
    crosshair_t *crosshair = malloc(sizeof(crosshair_t));

    if (!crosshair)
        return NULL;
    crosshair->horizontal = sfRectangleShape_create();
    crosshair->vertical = sfRectangleShape_create();
    if (!crosshair->horizontal || !crosshair->vertical) {
        if (crosshair->horizontal)
            sfRectangleShape_destroy(crosshair->horizontal);
        if (crosshair->vertical)
            sfRectangleShape_destroy(crosshair->vertical);
        free(crosshair);
        return NULL;
    }
    return crosshair;
}

static void setup_horizontal_bar(sfRectangleShape *shape)
{
    sfRectangleShape_setSize(shape, (sfVector2f){15.0f, 2.0f});
    sfRectangleShape_setFillColor(shape, sfGreen);
    sfRectangleShape_setOrigin(shape, (sfVector2f){7.5f, 1.0f});
}

static void setup_vertical_bar(sfRectangleShape *shape)
{
    sfRectangleShape_setSize(shape, (sfVector2f){2.0f, 15.0f});
    sfRectangleShape_setFillColor(shape, sfGreen);
    sfRectangleShape_setOrigin(shape, (sfVector2f){1.0f, 7.5f});
}

static crosshair_t *create_crosshair(void)
{
    crosshair_t *crosshair = allocate_crosshair();

    if (!crosshair)
        return NULL;
    setup_horizontal_bar(crosshair->horizontal);
    setup_vertical_bar(crosshair->vertical);
    return crosshair;
}

void update_crosshair(crosshair_t *crosshair, sfRenderWindow *window)
{
    sfVector2u window_size = sfRenderWindow_getSize(window);
    float center_x = window_size.x / 2.0f;
    float center_y = window_size.y / 2.0f;

    if (!crosshair || !window)
        return;
    sfRectangleShape_setPosition(crosshair->horizontal,
        (sfVector2f){center_x, center_y});
    sfRectangleShape_setPosition(crosshair->vertical,
        (sfVector2f){center_x, center_y});
}

void draw_crosshair(crosshair_t *crosshair, sfRenderWindow *window)
{
    if (!crosshair || !window)
        return;
    sfRenderWindow_drawRectangleShape(window, crosshair->horizontal, NULL);
    sfRenderWindow_drawRectangleShape(window, crosshair->vertical, NULL);
}

static void destroy_crosshair(crosshair_t *crosshair)
{
    if (!crosshair)
        return;
    if (crosshair->horizontal)
        sfRectangleShape_destroy(crosshair->horizontal);
    if (crosshair->vertical)
        sfRectangleShape_destroy(crosshair->vertical);
    free(crosshair);
}

void init_game_components(game_context_t *ctx)
{
    if (!ctx)
        return;
    ctx->crosshair = create_crosshair();
    sfRenderWindow_setMouseCursorVisible(ctx->window, sfFalse);
}

void cleanup_game_components(game_context_t *ctx)
{
    if (!ctx)
        return;
    if (ctx->crosshair) {
        destroy_crosshair(ctx->crosshair);
        ctx->crosshair = NULL;
    }
    if (ctx->light_sprite) {
        sfSprite_destroy(ctx->light_sprite);
        ctx->light_sprite = NULL;
    }
    if (ctx->light_texture) {
        sfRenderTexture_destroy(ctx->light_texture);
        ctx->light_texture = NULL;
    }
    if (ctx->game_view) {
        sfView_destroy(ctx->game_view);
        ctx->game_view = NULL;
    }
    sfRenderWindow_setMouseCursorVisible(ctx->window, sfTrue);
}
