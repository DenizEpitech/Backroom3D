/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** setting_ui
*/

#include "wolf.h"
#include "menu.h"
#include "setting.h"
#include "utils.h"
#include <stdlib.h>

int init_background(background_t *background)
{
    return init_texture_sprite(background,
        "ressources/menu/settings/settings_background.png",
        (sfVector2f){0, 0});
}

static void draw_settings_elements(sfRenderWindow *window,
    settings_t *settings)
{
    sfRenderWindow_clear(window, sfBlack);
    if (settings->background.sprite)
        sfRenderWindow_drawSprite(window, settings->background.sprite, NULL);
    if (settings->resolution1_button)
        draw_button(window, settings->resolution1_button);
    if (settings->resolution2_button)
        draw_button(window, settings->resolution2_button);
    if (settings->volume.bar)
        sfRenderWindow_drawRectangleShape(window, settings->volume.bar, NULL);
    if (settings->volume.knob)
        sfRenderWindow_drawCircleShape(window, settings->volume.knob, NULL);
    sfRenderWindow_display(window);
    return;
}

static void change_resolution(sfRenderWindow **window,
    unsigned int width, unsigned int height)
{
    sfView *view = NULL;

    if (!window || !*window)
        return;
    sfRenderWindow_close(*window);
    sfRenderWindow_destroy(*window);
    *window = create_window_util(width, height, "WOLFENSTEIN3D",
        sfResize | sfClose);
    if (*window) {
        view = sfView_createFromRect((sfFloatRect){0, 0, 1920, 1080});
        sfView_setCenter(view, (sfVector2f){960, 540});
        sfRenderWindow_setView(*window, view);
        sfView_destroy(view);
    }
    return;
}

void set_resolution(sfRenderWindow **window, sfBool high_resolution)
{
    if (high_resolution) {
        change_resolution(window, WIDTH, HEIGHT);
    } else {
        change_resolution(window, 960, 540);
    }
    return;
}

void handle_mouse_interactions(settings_interface_context_t *ctx)
{
    sfVector2i mouse_screen_pos = {0};
    sfVector2f mouse_world_pos = {0};

    mouse_screen_pos = sfMouse_getPositionRenderWindow(*(ctx->window));
    mouse_world_pos = convert_mouse_position(*(ctx->window),
    mouse_screen_pos);
    ctx->mouse_pos = (sfVector2i){(int)mouse_world_pos.x,
        (int)mouse_world_pos.y};
    if (ctx->settings->volume.dragging)
        handle_slider_interaction(&ctx->settings->volume,
            ctx->settings->music, ctx->mouse_pos);
    return;
}

void process_ui_and_events(settings_interface_context_t *ctx)
{
    settings_event_context_t event_ctx = {0};

    event_ctx.window = ctx->window;
    event_ctx.event = ctx->event;
    event_ctx.settings = ctx->settings;
    event_ctx.mouse_pos = ctx->mouse_pos;
    event_ctx.is_open = ctx->is_open;
    event_ctx.state = ctx->state;
    event_ctx.resolution_changed = ctx->resolution_changed;
    process_settings_events(&event_ctx);
    update_button_states(ctx->settings, ctx->mouse_pos);
    draw_settings_elements(*(ctx->window), ctx->settings);
    return;
}

void update_button_states(settings_t *settings, sfVector2i mouse_pos)
{
    update_button_state(settings->resolution1_button, mouse_pos, sfFalse);
    update_button_state(settings->resolution2_button, mouse_pos, sfFalse);
    return;
}
