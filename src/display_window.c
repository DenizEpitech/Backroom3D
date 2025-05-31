/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d
** File description:
** display_window.c
*/

#include "wolf.h"

void close_window(sfRenderWindow *window, sfEvent event)
{
    if (event.type == sfEvtClosed) {
        sfRenderWindow_close(window);
    }
    return;
}

int window(void)
{
    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfEvent event = {0};
    sfRenderWindow* window = sfRenderWindow_create
    (mode, "WOLFENSTEIN3D", sfResize | sfClose, NULL);

    if (!window) {
        return EXIT_ERROR;
    }
    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            close_window(window, event);
        }
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window);
    }
    sfRenderWindow_destroy(window);
    return EXIT_SUCCESS;
}
