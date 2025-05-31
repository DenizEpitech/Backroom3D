/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** setting_main
*/

#include "wolf.h"
#include "menu.h"
#include "setting.h"
#include "utils.h"
#include <stdlib.h>

static settings_t *create_settings(sfMusic *menu_music)
{
    settings_t *settings = malloc(sizeof(settings_t));

    if (!settings)
        return NULL;
    settings->background.texture = NULL;
    settings->background.sprite = NULL;
    settings->resolution1_button = NULL;
    settings->resolution2_button = NULL;
    settings->volume.bar = NULL;
    settings->volume.knob = NULL;
    settings->volume.level = 1.0f;
    settings->volume.bar_pos = (sfVector2f){650, 700};
    settings->volume.bar_width = 600.0f;
    settings->volume.dragging = sfFalse;
    settings->music = menu_music;
    return settings;
}

void destroy_settings(settings_t *settings)
{
    if (!settings)
        return;
    destroy_texture_sprite(&settings->background);
    if (settings->resolution1_button)
        destroy_button(settings->resolution1_button);
    if (settings->resolution2_button)
        destroy_button(settings->resolution2_button);
    free_volume_controls(&settings->volume);
    free(settings);
    return;
}

static void init_settings_interface_context
(settings_interface_context_t *ctx, context_init_params_t *params)
{
    ctx->window = params->window;
    ctx->event = params->event;
    ctx->settings = params->settings;
    ctx->is_open = params->is_open;
    ctx->state = params->state;
    ctx->resolution_changed = params->resolution_changed;
    return;
}

static int init_resolution_buttons(settings_t *settings)
{
    settings->resolution1_button = create_button(
        "ressources/menu/settings/resolution1.png",
        (sfVector2f){480, 380},
        (sfVector2f){450, 120});
    if (!settings->resolution1_button)
        return EXIT_ERROR;
    settings->resolution2_button = create_button(
        "ressources/menu/settings/resolution2.png",
        (sfVector2f){1000, 380},
        (sfVector2f){450, 120});
    if (!settings->resolution2_button) {
        destroy_button(settings->resolution1_button);
        settings->resolution1_button = NULL;
        return EXIT_ERROR;
    }
    return EXIT_SUCCESS;
}

static int settings_loop(sfRenderWindow **window, sfEvent event,
    game_state_t *state, settings_t *settings)
{
    sfBool is_open = sfTrue;
    sfBool resolution_changed = sfFalse;
    settings_interface_context_t ctx = {0};
    context_init_params_t params = {0};

    while (is_open && sfRenderWindow_isOpen(*window)) {
        params.window = window;
        params.event = event;
        params.settings = settings;
        params.is_open = &is_open;
        params.state = state;
        params.resolution_changed = &resolution_changed;
        init_settings_interface_context(&ctx, &params);
        handle_mouse_interactions(&ctx);
        process_ui_and_events(&ctx);
    }
    return resolution_changed ? 1 : 0;
}

int display_settings(sfRenderWindow **window, sfEvent event,
    game_state_t *state, sfMusic *menu_music)
{
    settings_t *settings = NULL;
    int result = 0;

    if (!window || !*window || !state)
        return EXIT_ERROR;
    settings = create_settings(menu_music);
    if (!settings)
        return EXIT_ERROR;
    if (init_background(&settings->background) != EXIT_SUCCESS ||
        init_resolution_buttons(settings) != EXIT_SUCCESS ||
        init_volume_controls(&settings->volume) != EXIT_SUCCESS) {
        destroy_settings(settings);
        return EXIT_ERROR;
    }
    result = settings_loop(window, event, state, settings);
    destroy_settings(settings);
    if (result)
        return display_settings(window, event, state, menu_music);
    return EXIT_SUCCESS;
}
