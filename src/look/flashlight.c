/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d
** File description:
** flashlight.c
*/

#include "wolf.h"

static void draw_dark_background(game_context_t *ctx, sfVector2u win_size)
{
    sfRectangleShape *dark_rect = sfRectangleShape_create();
    sfColor color = {0};

    sfRectangleShape_setSize(dark_rect, (sfVector2f){win_size.x, win_size.y});
    color = sfColor_fromRGBA(90, 90, 90, 200);
    sfRectangleShape_setFillColor(dark_rect, color);
    sfRenderTexture_clear(ctx->light_texture, sfTransparent);
    sfRenderTexture_drawRectangleShape(ctx->light_texture, dark_rect, NULL);
    sfRectangleShape_destroy(dark_rect);
}

static void manage_flashlight_circle(sfCircleShape *circle, sfVector2f center,
    float radius, int alpha)
{
    sfColor color = {0};

    sfCircleShape_setRadius(circle, radius);
    sfCircleShape_setOrigin(circle, (sfVector2f){radius, radius});
    sfCircleShape_setPosition(circle, center);
    sfCircleShape_setScale(circle, (sfVector2f){1.6f, 1.0f});
    sfCircleShape_setRotation(circle, 0.0f);
    color = sfColor_fromRGBA(100, 100, 100, alpha);
    sfCircleShape_setFillColor(circle, color);
}

static void draw_flashlight(sfRenderTexture *texture, sfVector2f center,
    float max_radius, int layers)
{
    sfCircleShape *circle = sfCircleShape_create();
    sfRenderStates states = {
        .blendMode = sfBlendMultiply,
        .transform = sfTransform_Identity,
        .texture = NULL,
        .shader = NULL
    };
    float radius = 0.0f;
    float t = 0.0f;
    int alpha = 0;

    for (int i = 0; i < layers; ++i) {
        t = (float)i / (float)layers;
        radius = t * max_radius;
        alpha = (int)(32 * (1.0f - t));
        manage_flashlight_circle(circle, center, radius, alpha);
        sfRenderTexture_drawCircleShape(texture, circle, &states);
    }
    sfCircleShape_destroy(circle);
}

void draw_flashlight_overlay(game_context_t *ctx, float max_radius)
{
    sfVector2u win_size = sfRenderWindow_getSize(ctx->window);
    sfVector2f center = {win_size.x / 2.0f, win_size.y / 2.0f};
    int layers = 60;

    draw_dark_background(ctx, win_size);
    draw_flashlight(ctx->light_texture, center, max_radius, layers);
    sfRenderTexture_display(ctx->light_texture);
}
