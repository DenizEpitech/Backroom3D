/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d-donata.contant
** File description:
** init_health.c
*/

#include "wolf.h"
#include "enemy.h"
#include <SFML/Graphics.h>
#include <math.h>
#include <stdlib.h>

health_sprite_system_t *init_health_sprite_system(sfSprite **sprites,
    int max_health)
{
    health_sprite_system_t *system = malloc(sizeof(health_sprite_system_t));

    if (!system) {
        return NULL;
    }
    system->health_sprites = sprites;
    system->max_health = max_health;
    system->current_health = max_health;
    system->is_visible = 1;
    system->display_timer = 999.0f;
    system->current_sprite_index = 0;
    system->position = (sfVector2f){50, 50};
    return system;
}

sfSprite *load_health_sprite(const char *path)
{
    sfTexture *texture = sfTexture_createFromFile(path, NULL);
    sfSprite *sprite = NULL;

    if (!texture) {
        return NULL;
    }
    sprite = sfSprite_create();
    if (!sprite) {
        sfTexture_destroy(texture);
        return NULL;
    }
    sfSprite_setTexture(sprite, texture, sfTrue);
    return sprite;
}

static int init_health_structure(game_context_t *ctx)
{
    ctx->health_system = malloc(sizeof(health_sprite_system_t));
    if (!ctx->health_system)
        return EXIT_ERROR;
    ctx->health_system->health_sprites = malloc(5 * sizeof(sfSprite *));
    ctx->health_system->health_textures = malloc(5 * sizeof(sfTexture *));
    if (!ctx->health_system->health_sprites
        || !ctx->health_system->health_textures)
        return EXIT_ERROR;
    ctx->health_system->max_health = 100;
    ctx->health_system->current_health = 100;
    ctx->health_system->is_visible = 1;
    ctx->health_system->display_timer = 999.0f;
    ctx->health_system->current_sprite_index = 0;
    ctx->health_system->position = (sfVector2f){50, 50};
    return EXIT_SUCCESS;
}

static int load_health_sprites(game_context_t *ctx)
{
    const char *sprite_paths[5] = {
        "././ressources/game/life/life_one.png",
        "././ressources/game/life/life_two.png",
        "././ressources/game/life/life_three.png",
        "././ressources/game/life/life_four.png",
        "././ressources/game/life/life_five.png"
    };

    for (int i = 0; i < 5; i++) {
        ctx->health_system->health_textures[i] =
            sfTexture_createFromFile(sprite_paths[i], NULL);
        if (!ctx->health_system->health_textures[i])
            return EXIT_ERROR;
        ctx->health_system->health_sprites[i] = sfSprite_create();
        if (!ctx->health_system->health_sprites[i])
            return EXIT_ERROR;
        sfSprite_setTexture(ctx->health_system->health_sprites[i],
            ctx->health_system->health_textures[i], sfTrue);
    }
    return EXIT_SUCCESS;
}

int setup_health_system(game_context_t *ctx)
{
    if (init_health_structure(ctx) != EXIT_SUCCESS)
        return EXIT_ERROR;
    if (load_health_sprites(ctx) != EXIT_SUCCESS)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}
