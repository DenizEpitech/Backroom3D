/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** weapon_animation_render
*/

#include "weapon.h"
#include <stdlib.h>
#include <stdio.h>

char *create_texture_path(int index)
{
    int path_len = snprintf(NULL, 0,
        "ressources/game/weapon/weapon%d.png", index);
    char *path = malloc((path_len + 1) * sizeof(char));

    if (!path)
        return NULL;
    snprintf(path, path_len + 1,
        "ressources/game/weapon/weapon%d.png", index);
    return path;
}

int create_weapon_sprites(weapon_t *weapon)
{
    sfFloatRect bounds = {0};

    for (int i = 0; i < weapon->animation.total_frames; i++) {
        weapon->animation.sprites[i] = sfSprite_create();
        if (!weapon->animation.sprites[i])
            return EXIT_FAILURE;
        sfSprite_setTexture(weapon->animation.sprites[i],
            weapon->animation.textures[i], sfTrue);
        bounds = sfSprite_getLocalBounds
        (weapon->animation.sprites[i]);
        sfSprite_setOrigin(weapon->animation.sprites[i],
            (sfVector2f){bounds.width / 2, bounds.height});
    }
    return EXIT_SUCCESS;
}

void configure_weapon_sprites(weapon_t *weapon)
{
    for (int i = 0; i < weapon->animation.total_frames; i++) {
        sfSprite_setScale(weapon->animation.sprites[i], weapon->display.scale);
        sfSprite_setPosition(weapon->animation.sprites[i],
            weapon->display.position);
    }
}

void update_weapon_position(weapon_t *weapon, sfRenderWindow *window)
{
    sfVector2u win_size = sfRenderWindow_getSize(window);
    sfFloatRect bounds = {0};

    if (!weapon || !weapon->animation.sprites)
        return;
    bounds = sfSprite_getLocalBounds(weapon->animation.sprites[0]);
    weapon->display.position = (sfVector2f){win_size.x / 2.0f,
        win_size.y - (bounds.height * weapon->display.scale.y)};
    for (int i = 0; i < weapon->animation.total_frames; i++) {
        if (weapon->animation.sprites[i])
            sfSprite_setPosition(weapon->animation.sprites[i],
                weapon->display.position);
    }
}
