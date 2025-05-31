/*
** EPITECH PROJECT, 2025
** wold3d
** File description:
** menu_manager
*/

#include "menu.h"
#include "wolf.h"
#include <stdlib.h>

void free_menu_resources(menu_t *menu)
{
    if (menu->background)
        sfSprite_destroy(menu->background);
    if (menu->background_texture)
        sfTexture_destroy(menu->background_texture);
    if (menu->start_button)
        destroy_button(menu->start_button);
    if (menu->settings_button)
        destroy_button(menu->settings_button);
    if (menu->exit_button)
        destroy_button(menu->exit_button);
    if (menu->music) {
        sfMusic_stop(menu->music);
        sfMusic_destroy(menu->music);
    }
    free(menu);
    return;
}

static sfVector2i get_mouse_position_in_view(sfRenderWindow *window)
{
    sfVector2i mouse_screen = sfMouse_getPositionRenderWindow(window);
    sfVector2f mouse_world = sfRenderWindow_mapPixelToCoords(window,
        mouse_screen, sfRenderWindow_getView(window));

    return (sfVector2i){(int)mouse_world.x, (int)mouse_world.y};
}

static void handle_button_clicks(menu_t *menu, game_state_t *state,
    sfRenderWindow *window)
{
    if (menu->start_button->state == CLICKED) {
        *state = GAME;
    }
    if (menu->settings_button->state == CLICKED) {
        *state = SETTINGS;
    }
    if (menu->exit_button->state == CLICKED) {
        *state = EXIT;
        sfRenderWindow_close(window);
    }
}

void handle_menu_events(sfRenderWindow *window, sfEvent event,
    menu_t *menu, game_state_t *state)
{
    sfVector2i mouse_pos = get_mouse_position_in_view(window);
    sfBool mouse_clicked = (event.type == sfEvtMouseButtonPressed &&
    event.mouseButton.button == sfMouseLeft);

    update_button_state(menu->start_button, mouse_pos, mouse_clicked);
    update_button_state(menu->settings_button, mouse_pos, mouse_clicked);
    update_button_state(menu->exit_button, mouse_pos, mouse_clicked);
    if (mouse_clicked)
        handle_button_clicks(menu, state, window);
    return;
}
