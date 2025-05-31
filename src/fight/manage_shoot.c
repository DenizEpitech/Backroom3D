/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** manage_sound.c
*/

#include "wolf.h"
#include <SFML/Window.h>
#include <stdlib.h>
#include <stdio.h>

void init_shoot_sound(sfSound **sound, sfSoundBuffer **buffer)
{
    *sound = sfSound_create();
    *buffer = sfSoundBuffer_createFromFile("./ressources/song/tir_song.ogg");
    if ((*buffer) == NULL) {
        sfSound_destroy(*sound);
        *sound = NULL;
        return;
    }
    sfSound_setBuffer(*sound, *buffer);
    sfSound_setVolume(*sound, 100);
    return;
}

void destroy_shoot_sound(sfSound *sound, sfSoundBuffer *buffer)
{
    if (sound) {
        sfSound_stop(sound);
        sfSound_destroy(sound);
    }
    if (buffer) {
        sfSoundBuffer_destroy(buffer);
    }
    return;
}

void play_shoot_sound(sfSound* sound)
{
    if (sound) {
        sfSound_play(sound);
    }
    return;
}

void update_shoot_effect(shoot_effect_t *shoot)
{
    if (shoot->alpha > 0) {
        shoot->alpha -= 5.0f;
        if (shoot->alpha < 0)
            shoot->alpha = 0;
    }
    return;
}

void player_shoot_at_position(player_t *player,
    projectile_t **projectile_list, sfVector2i mousePos)
{
    project_info_t info = {0};
    sfVector2u win_size = sfRenderWindow_getSize(player->context->window);
    float screenCenterX = win_size.x / 2.0f;
    float screenCenterY = win_size.y / 2.0f;
    float angleOffset = atan2f(mousePos.y - screenCenterY,
        mousePos.x - screenCenterX);
    float shootAngle = player->angle + angleOffset;

    if (!player || !player->context || !player->context->window
        || !projectile_list)
        return;
    info.x = player->x;
    info.y = player->y;
    info.dir.x = cos(shootAngle);
    info.dir.y = sin(shootAngle);
    info.speed = 5.0f;
    info.damage = 10;
    add_projectile(projectile_list, info);
}
