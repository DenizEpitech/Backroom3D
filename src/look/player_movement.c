/*
** EPITECH PROJECT, 2025
** Wolf3d
** File description:
** player_movement_sprint.c
*/
#include "wolf.h"
#include <SFML/Window.h>
#include <stdlib.h>

static void move_player(player_t *player, float new_x, float new_y)
{
    if (player->data == NULL) {
        return;
    }
    if (!is_wall(player->data, new_x, new_y)) {
        player->x = new_x;
        player->y = new_y;
    }
    return;
}

static sprint_state_t *get_sprint_state(void)
{
    static sprint_state_t state = {0};

    return &state;
}

static void check_sprint_toggle(void)
{
    static int last_v_state = 0;
    sprint_state_t *state = get_sprint_state();
    int current_v_state = sfKeyboard_isKeyPressed(sfKeyV);

    if (current_v_state && !last_v_state) {
        state->is_sprint_active = !state->is_sprint_active;
    }
    last_v_state = current_v_state;
}

static float get_movement_speed(void)
{
    sprint_state_t *state = get_sprint_state();
    float base_speed = PLAYER_MOVE_SPEED;

    if (state->is_sprint_active) {
        return base_speed * SPEED_MULTIPLIER;
    }
    return base_speed;
}

static void handle_up_and_down(player_t *player, float move_speed,
    float new_x, float new_y)
{
    if (sfKeyboard_isKeyPressed(sfKeyUp) || sfKeyboard_isKeyPressed(sfKeyZ)) {
        new_x = player->x + cos(player->angle) * move_speed;
        new_y = player->y + sin(player->angle) * move_speed;
        move_player(player, new_x, new_y);
    }
    if (sfKeyboard_isKeyPressed(sfKeyDown) ||
        sfKeyboard_isKeyPressed(sfKeyS)) {
        new_x = player->x - cos(player->angle) * move_speed;
        new_y = player->y - sin(player->angle) * move_speed;
        move_player(player, new_x, new_y);
    }
    return;
}

static void handle_strafe(player_t *player, float move_speed)
{
    float strafe_x = 0.0f;
    float strafe_y = 0.0f;

    if (sfKeyboard_isKeyPressed(sfKeyA) || sfKeyboard_isKeyPressed(sfKeyQ)) {
        strafe_x = player->x - cos(player->angle + M_PI_2) * move_speed;
        strafe_y = player->y - sin(player->angle + M_PI_2) * move_speed;
        move_player(player, strafe_x, strafe_y);
    }
    if (sfKeyboard_isKeyPressed(sfKeyE) || sfKeyboard_isKeyPressed(sfKeyD)) {
        strafe_x = player->x + cos(player->angle + M_PI_2) * move_speed;
        strafe_y = player->y + sin(player->angle + M_PI_2) * move_speed;
        move_player(player, strafe_x, strafe_y);
    }
}

static void apply_mouse_rotation(player_t *player, int delta_x)
{
    float mouse_sensitivity = 0.002f;

    player->angle += delta_x * mouse_sensitivity;
    player->angle = fmod(player->angle, (2 * M_PI));
    if (player->angle < 0)
        player->angle += (2 * M_PI);
}

static void recenter_mouse_if_needed(sfRenderWindow *window,
    sfVector2i current_pos, sfVector2i *last_pos)
{
    sfVector2u window_size = sfRenderWindow_getSize(window);
    sfVector2i center = {window_size.x / 2, window_size.y / 2};

    if (abs(current_pos.x - center.x) > 100 ||
        abs(current_pos.y - center.y) > 100) {
        sfMouse_setPositionRenderWindow(center, window);
        *last_pos = center;
    } else {
        *last_pos = current_pos;
    }
}

static void handle_mouse_rotation(player_t *player)
{
    static sfVector2i last_mouse_pos = {-1, -1};
    sfVector2i current_mouse_pos;
    int delta_x = 0;

    if (!player->context || !player->context->window)
        return;
    current_mouse_pos =
        sfMouse_getPositionRenderWindow(player->context->window);
    if (last_mouse_pos.x == -1 && last_mouse_pos.y == -1) {
        last_mouse_pos = current_mouse_pos;
        return;
    }
    delta_x = current_mouse_pos.x - last_mouse_pos.x;
    apply_mouse_rotation(player, delta_x);
    recenter_mouse_if_needed(player->context->window,
        current_mouse_pos, &last_mouse_pos);
}

void update_player(player_t *player)
{
    float move_speed = 0.0f;
    float new_x = 0.0f;
    float new_y = 0.0f;

    check_sprint_toggle();
    move_speed = get_movement_speed();
    handle_up_and_down(player, move_speed, new_x, new_y);
    handle_strafe(player, move_speed);
    handle_mouse_rotation(player);
    if (sfKeyboard_isKeyPressed(sfKeyLeft))
        player->angle -= PLAYER_ROTATION_SPEED;
    if (sfKeyboard_isKeyPressed(sfKeyRight))
        player->angle += PLAYER_ROTATION_SPEED;
    return;
}
