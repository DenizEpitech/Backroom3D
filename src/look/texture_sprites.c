/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** texture_sprites
*/

#include "wolf.h"
#include "texture_renderer.h"
#include <stdio.h>
#include <stdlib.h>

void create_wall_sprite(texture_manager_t *manager, int index)
{
    if (!manager)
        return;
    if (!manager->wall_textures[index])
        return;
    manager->wall_sprites[index] = sfSprite_create();
    if (manager->wall_sprites[index])
        sfSprite_setTexture(manager->wall_sprites[index],
            manager->wall_textures[index], sfTrue);
    return;
}

static void create_wall_sprites(texture_manager_t *manager)
{
    int i = 0;

    if (!manager)
        return;
    for (i = 0; i < MAX_TEXTURES; i++)
        create_wall_sprite(manager, i);
    return;
}

static void create_floor_ceiling_sprites(texture_manager_t *manager)
{
    if (!manager)
        return;
    if (manager->floor_texture) {
        manager->floor_sprite = sfSprite_create();
        if (manager->floor_sprite) {
            sfSprite_setTexture(manager->floor_sprite,
                manager->floor_texture, sfTrue);
        }
    }
    if (manager->ceiling_texture) {
        manager->ceiling_sprite = sfSprite_create();
        if (manager->ceiling_sprite) {
            sfSprite_setTexture(manager->ceiling_sprite,
                manager->ceiling_texture, sfTrue);
        }
    }
    return;
}

void create_sprites(texture_manager_t *manager)
{
    if (!manager)
        return;
    create_wall_sprites(manager);
    create_floor_ceiling_sprites(manager);
    return;
}
