/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** fps_display
*/

#include "fps.h"
#include "wolf.h"
#include <stdlib.h>
#include <stdio.h>

fps_info_t *create_fps_display(void)
{
    fps_info_t *fps_info = malloc(sizeof(fps_info_t));

    if (!fps_info)
        return NULL;
    fps_info->fps_clock = sfClock_create();
    fps_info->font = sfFont_createFromFile("ressources/font.ttf");
    fps_info->fps_text = sfText_create();
    if (!fps_info->fps_clock || !fps_info->font || !fps_info->fps_text) {
        destroy_fps_display(fps_info);
        return NULL;
    }
    sfText_setFont(fps_info->fps_text, fps_info->font);
    sfText_setCharacterSize(fps_info->fps_text, 20);
    sfText_setFillColor(fps_info->fps_text, sfWhite);
    sfText_setPosition(fps_info->fps_text, (sfVector2f){WIDTH - 100, 10});
    fps_info->fps = 0.0f;
    fps_info->frame_count = 0;
    fps_info->elapsed_time = 0.0f;
    return fps_info;
}

void update_fps_display(fps_info_t *fps_info)
{
    char fps_str[32] = {0};

    if (!fps_info)
        return;
    fps_info->frame_count++;
    fps_info->elapsed_time += sfTime_asSeconds
    (sfClock_restart(fps_info->fps_clock));
    if (fps_info->elapsed_time >= 0.5f) {
        fps_info->fps = fps_info->frame_count / fps_info->elapsed_time;
        fps_info->frame_count = 0;
        fps_info->elapsed_time = 0.0f;
        snprintf(fps_str, sizeof(fps_str), "FPS: %.1f", fps_info->fps);
        sfText_setString(fps_info->fps_text, fps_str);
    }
    return;
}

void draw_fps_display(sfRenderWindow *window, fps_info_t *fps_info)
{
    if (!window || !fps_info || !fps_info->fps_text)
        return;
    sfRenderWindow_drawText(window, fps_info->fps_text, NULL);
    return;
}

void destroy_fps_display(fps_info_t *fps_info)
{
    if (!fps_info)
        return;
    if (fps_info->fps_clock)
        sfClock_destroy(fps_info->fps_clock);
    if (fps_info->fps_text)
        sfText_destroy(fps_info->fps_text);
    if (fps_info->font)
        sfFont_destroy(fps_info->font);
    free(fps_info);
    return;
}
