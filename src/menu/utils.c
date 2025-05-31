/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** utils
*/

#include "wolf.h"
#include "menu.h"
#include <stdlib.h>

sfRenderWindow *create_window_util(unsigned int width, unsigned int height,
    const char *title, sfUint32 style)
{
    sfVideoMode mode = {width, height, 32};
    sfVideoMode desktop = {0};
    int pos_x = 0;
    int pos_y = 0;
    sfRenderWindow *window = sfRenderWindow_create(mode, title, style, NULL);

    if (window) {
        desktop = sfVideoMode_getDesktopMode();
        pos_x = (desktop.width - width) / 2;
        pos_y = (desktop.height - height) / 2;
        sfRenderWindow_setPosition(window, (sfVector2i){pos_x, pos_y});
    }
    return window;
}

int init_texture_sprite(background_t *background, const char *texture_path,
    sfVector2f position)
{
    background->texture = sfTexture_createFromFile(texture_path, NULL);
    if (!background->texture)
        return EXIT_ERROR;
    background->sprite = sfSprite_create();
    if (!background->sprite) {
        sfTexture_destroy(background->texture);
        background->texture = NULL;
        return EXIT_ERROR;
    }
    sfSprite_setTexture(background->sprite, background->texture, sfTrue);
    sfSprite_setPosition(background->sprite, position);
    return EXIT_SUCCESS;
}

void destroy_texture_sprite(background_t *background)
{
    if (background->sprite)
        sfSprite_destroy(background->sprite);
    if (background->texture)
        sfTexture_destroy(background->texture);
    background->sprite = NULL;
    background->texture = NULL;
    return;
}

void handle_generic_events(sfRenderWindow *window, sfEvent event,
    game_state_t *state)
{
    if (event.type == sfEvtClosed) {
        sfRenderWindow_close(window);
        *state = EXIT;
    }
    if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape)
        *state = MENU;
    return;
}
