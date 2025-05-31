/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** setting_events
*/

#include "wolf.h"
#include "menu.h"
#include "setting.h"
#include "utils.h"
#include <stdlib.h>

sfVector2f convert_mouse_position(sfRenderWindow *window,
    sfVector2i mouse_pos)
{
    return sfRenderWindow_mapPixelToCoords(window, mouse_pos,
        sfRenderWindow_getView(window));
}

static void handle_volume_slider_click(mouse_button_context_t *ctx)
{
    if (ctx->event.type != sfEvtMouseButtonPressed ||
        ctx->event.mouseButton.button != sfMouseLeft)
        return;
    if (ctx->mouse_pos.y >= ctx->settings->volume.bar_pos.y - 20 &&
        ctx->mouse_pos.y <= ctx->settings->volume.bar_pos.y + 20 &&
        ctx->mouse_pos.x >= ctx->settings->volume.bar_pos.x &&
        ctx->mouse_pos.x <= ctx->settings->volume.bar_pos.x +
        ctx->settings->volume.bar_width) {
        ctx->settings->volume.dragging = sfTrue;
        handle_slider_interaction(&ctx->settings->volume,
            ctx->settings->music, ctx->mouse_pos);
    }
    return;
}

static void handle_resolution_buttons(mouse_button_context_t *ctx)
{
    if (ctx->event.type != sfEvtMouseButtonPressed ||
        ctx->event.mouseButton.button != sfMouseLeft)
        return;
    update_button_state(ctx->settings->resolution1_button,
        ctx->mouse_pos, sfTrue);
    update_button_state(ctx->settings->resolution2_button,
        ctx->mouse_pos, sfTrue);
    if (ctx->settings->resolution1_button->state == CLICKED) {
        set_resolution(ctx->window, sfTrue);
        *ctx->is_open = sfFalse;
        *ctx->resolution_changed = sfTrue;
        *ctx->state = SETTINGS;
    } else if (ctx->settings->resolution2_button->state == CLICKED) {
        set_resolution(ctx->window, sfFalse);
        *ctx->is_open = sfFalse;
        *ctx->resolution_changed = sfTrue;
        *ctx->state = SETTINGS;
    }
    return;
}

static void handle_mouse_button_press(mouse_button_context_t *ctx)
{
    if (ctx->event.type != sfEvtMouseButtonPressed ||
        ctx->event.mouseButton.button != sfMouseLeft)
        return;
    handle_volume_slider_click(ctx);
    handle_resolution_buttons(ctx);
    return;
}

void handle_mouse_button_release(sfEvent event, settings_t *settings)
{
    if (event.type == sfEvtMouseButtonReleased &&
        event.mouseButton.button == sfMouseLeft)
        settings->volume.dragging = sfFalse;
    return;
}

sfBool handle_system_events(settings_event_context_t *ctx)
{
    if (ctx->event.type == sfEvtClosed) {
        sfRenderWindow_close(*ctx->window);
        *ctx->is_open = sfFalse;
        return sfTrue;
    }
    if (ctx->event.type == sfEvtKeyPressed &&
        ctx->event.key.code == sfKeyEscape) {
        *ctx->is_open = sfFalse;
        *ctx->state = MENU;
    }
    return sfFalse;
}

void handle_mouse_events(settings_event_context_t *ctx)
{
    mouse_button_context_t mouse_ctx = {0};

    mouse_ctx.window = ctx->window;
    mouse_ctx.event = ctx->event;
    mouse_ctx.settings = ctx->settings;
    mouse_ctx.mouse_pos = ctx->mouse_pos;
    mouse_ctx.is_open = ctx->is_open;
    mouse_ctx.state = ctx->state;
    mouse_ctx.resolution_changed = ctx->resolution_changed;
    handle_mouse_button_press(&mouse_ctx);
    handle_mouse_button_release(ctx->event, ctx->settings);
    return;
}

static sfBool handle_specific_event(settings_event_context_t *ctx)
{
    if (handle_system_events(ctx)) {
        return sfTrue;
    }
    handle_mouse_events(ctx);
    return sfFalse;
}

void process_settings_events(settings_event_context_t *ctx)
{
    sfBool should_break = {0};

    while (sfRenderWindow_pollEvent(*ctx->window, &ctx->event)) {
        should_break = handle_specific_event(ctx);
        if (should_break)
            break;
    }
    return;
}
