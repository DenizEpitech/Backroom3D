/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu
*/

#include "menu.h"
#include "wolf.h"
#include <stdlib.h>

static menu_t *init_menu_struct(void)
{
    menu_t *menu = malloc(sizeof(menu_t));

    if (!menu)
        return NULL;
    menu->background = NULL;
    menu->background_texture = NULL;
    menu->start_button = NULL;
    menu->settings_button = NULL;
    menu->exit_button = NULL;
    menu->music = NULL;
    return menu;
}

static int create_menu_background(menu_t *menu)
{
    sfFloatRect bounds = {0};

    menu->background_texture = sfTexture_createFromFile
    ("ressources/menu/fond_menu.png", NULL);
    if (!menu->background_texture)
        return EXIT_ERROR;
    menu->background = sfSprite_create();
    if (!menu->background)
        return EXIT_ERROR;
    sfSprite_setTexture(menu->background, menu->background_texture, sfTrue);
    bounds = sfSprite_getGlobalBounds(menu->background);
    sfSprite_setOrigin(menu->background,
        (sfVector2f){bounds.width / 2, bounds.height / 2});
    sfSprite_setPosition(menu->background, (sfVector2f){960, 540});
    return EXIT_SUCCESS;
}

static int create_start_button(menu_t *menu)
{
    menu->start_button = create_button("ressources/menu/start.png",
    (sfVector2f){960 - 250, 450}, (sfVector2f){500, 150});
    if (!menu->start_button)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

static int create_settings_button(menu_t *menu)
{
    menu->settings_button = create_button
    ("ressources/menu/setting_button.png",
    (sfVector2f){10, 20}, (sfVector2f){300, 100});
    if (!menu->settings_button)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

static int create_exit_button(menu_t *menu)
{
    menu->exit_button = create_button("ressources/menu/exit.png",
    (sfVector2f){960 - 250, 650}, (sfVector2f){500, 150});
    if (!menu->exit_button)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

int init_menu_elements(menu_t *menu)
{
    if (create_menu_background(menu) != EXIT_SUCCESS)
        return EXIT_ERROR;
    if (create_start_button(menu) != EXIT_SUCCESS)
        return EXIT_ERROR;
    if (create_settings_button(menu) != EXIT_SUCCESS)
        return EXIT_ERROR;
    if (create_exit_button(menu) != EXIT_SUCCESS)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

menu_t *create_menu(void)
{
    menu_t *menu = init_menu_struct();

    if (!menu)
        return NULL;
    if (init_menu_elements(menu) != EXIT_SUCCESS) {
        free_menu_resources(menu);
        return NULL;
    }
    if (music(&menu->music) == EXIT_ERROR) {
        free_menu_resources(menu);
        return NULL;
    }
    return menu;
}

void display_menu(sfRenderWindow *window, menu_t *menu)
{
    sfRenderWindow_drawSprite(window, menu->background, NULL);
    draw_button(window, menu->start_button);
    draw_button(window, menu->settings_button);
    draw_button(window, menu->exit_button);
    return;
}
