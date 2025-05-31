/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** button
*/

#include "menu.h"
#include "wolf.h"
#include <stdlib.h>

button_t *create_button(const char *texture_path,
    sfVector2f position, sfVector2f size)
{
    button_t *button = malloc(sizeof(button_t));

    if (!button)
        return NULL;
    button->rect = sfRectangleShape_create();
    button->texture = sfTexture_createFromFile(texture_path, NULL);
    if (!button->rect || !button->texture) {
        destroy_button(button);
        return NULL;
    }
    button->position = position;
    button->size = size;
    button->state = NORMAL;
    sfRectangleShape_setPosition(button->rect, position);
    sfRectangleShape_setSize(button->rect, size);
    sfRectangleShape_setTexture(button->rect, button->texture, sfTrue);
    return button;
}

void destroy_button(button_t *button)
{
    if (!button)
        return;
    if (button->rect)
        sfRectangleShape_destroy(button->rect);
    if (button->texture)
        sfTexture_destroy(button->texture);
    free(button);
    return;
}

static void determine_button_state(button_t *button,
    sfVector2i mouse_pos, sfBool clicked)
{
    sfFloatRect bounds = sfRectangleShape_getGlobalBounds(button->rect);

    if (sfFloatRect_contains(&bounds, (float)mouse_pos.x,
    (float)mouse_pos.y)) {
        if (clicked) {
            button->state = CLICKED;
        }
        if (!clicked) {
            button->state = HOVER;
        }
    }
    if (!sfFloatRect_contains(&bounds, (float)mouse_pos.x,
    (float)mouse_pos.y)) {
        button->state = NORMAL;
    }
    return;
}

static void apply_button_effect(button_t *button, button_state_t old_state)
{
    if (old_state != button->state) {
        if (button->state == HOVER) {
            sfRectangleShape_setScale(button->rect,
                (sfVector2f){1.05f, 1.05f});
        }
        if (button->state == CLICKED) {
            sfRectangleShape_setScale(button->rect,
                (sfVector2f){0.95f, 0.95f});
        }
        if (button->state == NORMAL) {
            sfRectangleShape_setScale(button->rect,
                (sfVector2f){1.0f, 1.0f});
        }
    }
    return;
}

void update_button_state(button_t *button,
    sfVector2i mouse_pos, sfBool clicked)
{
    button_state_t old_state = button->state;

    determine_button_state(button, mouse_pos, clicked);
    apply_button_effect(button, old_state);
    return;
}

void draw_button(sfRenderWindow *window, button_t *button)
{
    sfRenderWindow_drawRectangleShape(window, button->rect, NULL);
    return;
}

void destroy_menu(menu_t *menu)
{
    if (!menu)
        return;
    free_menu_resources(menu);
    return;
}
