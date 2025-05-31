/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** setting_volume
*/

#include "wolf.h"
#include "menu.h"
#include "setting.h"
#include "utils.h"
#include <stdlib.h>

static int init_volume_bar(volume_t *volume)
{
    volume->bar = sfRectangleShape_create();
    if (!volume->bar)
        return EXIT_ERROR;
    sfRectangleShape_setSize(volume->bar, (sfVector2f){volume->bar_width, 20});
    sfRectangleShape_setPosition(volume->bar, volume->bar_pos);
    sfRectangleShape_setFillColor(volume->bar, (sfColor){200, 200, 160, 255});
    return EXIT_SUCCESS;
}

static int init_volume_knob(volume_t *volume)
{
    float x_pos = 0.0f;

    volume->knob = sfCircleShape_create();
    if (!volume->knob)
        return EXIT_ERROR;
    sfCircleShape_setRadius(volume->knob, 20);
    sfCircleShape_setFillColor(volume->knob, (sfColor){180, 140, 100, 255});
    x_pos = volume->bar_pos.x + volume->level * volume->bar_width;
    sfCircleShape_setPosition(volume->knob,
        (sfVector2f){x_pos - 20, volume->bar_pos.y - 10});
    return EXIT_SUCCESS;
}

int init_volume_controls(volume_t *volume)
{
    if (init_volume_bar(volume) != EXIT_SUCCESS)
        return EXIT_ERROR;
    if (init_volume_knob(volume) != EXIT_SUCCESS) {
        sfRectangleShape_destroy(volume->bar);
        volume->bar = NULL;
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

void free_volume_controls(volume_t *volume)
{
    if (volume->bar)
        sfRectangleShape_destroy(volume->bar);
    if (volume->knob)
        sfCircleShape_destroy(volume->knob);
    volume->bar = NULL;
    volume->knob = NULL;
    return;
}

void update_volume(volume_t *volume, sfMusic *music,
    float new_volume)
{
    float x_pos = 0.0f;

    if (new_volume < 0.0f)
        new_volume = 0.0f;
    else if (new_volume > 1.0f)
        new_volume = 1.0f;
    volume->level = new_volume;
    x_pos = volume->bar_pos.x + volume->level * volume->bar_width;
    sfCircleShape_setPosition(volume->knob, (sfVector2f){x_pos - 20,
        volume->bar_pos.y - 10});
    if (music)
        sfMusic_setVolume(music, volume->level * 100);
    return;
}

void handle_slider_interaction(volume_t *volume,
    sfMusic *music, sfVector2i mouse_pos)
{
    float new_volume = 0.0f;
    float min_x = volume->bar_pos.x;
    float max_x = min_x + volume->bar_width;
    float min_y = volume->bar_pos.y - 20;
    float max_y = volume->bar_pos.y + 20;

    if (volume->dragging ||
        (mouse_pos.y >= min_y && mouse_pos.y <= max_y &&
        mouse_pos.x >= min_x && mouse_pos.x <= max_x)) {
        new_volume = (float)(mouse_pos.x - min_x) / volume->bar_width;
        update_volume(volume, music, new_volume);
    }
    return;
}
