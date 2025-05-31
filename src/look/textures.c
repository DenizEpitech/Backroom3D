/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** textures
*/

#include "wolf.h"
#include "texture_renderer.h"
#include <stdio.h>
#include <stdlib.h>

static void init_texture_arrays(texture_manager_t *manager)
{
    int i = 0;

    for (i = 0; i < MAX_TEXTURES; i++) {
        manager->wall_textures[i] = NULL;
        manager->wall_sprites[i] = NULL;
    }
    manager->floor_texture = NULL;
    manager->floor_sprite = NULL;
    manager->ceiling_texture = NULL;
    manager->ceiling_sprite = NULL;
    return;
}

static void load_wall_textures(texture_manager_t *manager)
{
    manager->wall_textures[0] = sfTexture_createFromFile
    ("ressources/map_ressource/wall_backroom.png", NULL);
    manager->wall_textures[1] = sfTexture_createFromFile
    ("ressources/map_ressource/wall_backroom.png", NULL);
    if (!manager->wall_textures[0]) {
        return;
    }
    return;
}

texture_manager_t *init_texture_manager(void)
{
    texture_manager_t *manager = malloc(sizeof(texture_manager_t));

    if (!manager)
        return NULL;
    init_texture_arrays(manager);
    load_wall_textures(manager);
    manager->floor_texture = sfTexture_createFromFile
    ("ressources/map_ressource/sol.png", NULL);
    manager->ceiling_texture = sfTexture_createFromFile
    ("ressources/map_ressource/ceiling.jpg", NULL);
    create_sprites(manager);
    return manager;
}

static void destroy_wall_textures(texture_manager_t *manager)
{
    int i = 0;

    for (i = 0; i < MAX_TEXTURES; i++) {
        if (manager->wall_sprites[i]) {
            sfSprite_destroy(manager->wall_sprites[i]);
            manager->wall_sprites[i] = NULL;
        }
        if (manager->wall_textures[i]) {
            sfTexture_destroy(manager->wall_textures[i]);
            manager->wall_textures[i] = NULL;
        }
    }
    return;
}

static void destroy_floor_ceiling_textures(texture_manager_t *manager)
{
    if (manager->floor_sprite) {
        sfSprite_destroy(manager->floor_sprite);
        manager->floor_sprite = NULL;
    }
    if (manager->floor_texture) {
        sfTexture_destroy(manager->floor_texture);
        manager->floor_texture = NULL;
    }
    if (manager->ceiling_sprite) {
        sfSprite_destroy(manager->ceiling_sprite);
        manager->ceiling_sprite = NULL;
    }
    if (manager->ceiling_texture) {
        sfTexture_destroy(manager->ceiling_texture);
        manager->ceiling_texture = NULL;
    }
    return;
}

void destroy_texture_manager(texture_manager_t *manager)
{
    if (!manager)
        return;
    destroy_wall_textures(manager);
    destroy_floor_ceiling_textures(manager);
    free(manager);
    return;
}
