/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** manage_project.c
*/

#include "wolf.h"
#include <SFML/Window.h>
#include <stdlib.h>

void add_projectile(projectile_t **head, project_info_t info)
{
    projectile_t *new_projectile = malloc(sizeof(projectile_t));

    if (!new_projectile)
        return;
    new_projectile->x = info.x;
    new_projectile->y = info.y;
    new_projectile->dir = info.dir;
    new_projectile->speed = info.speed;
    new_projectile->damage = info.damage;
    new_projectile->is_alive = 1;
    new_projectile->next = *head;
    *head = new_projectile;
    return;
}

static void handle_projectile_list(projectile_t **projectile_list,
    projectile_t *current, projectile_t *prev)
{
    if (prev == NULL) {
        *projectile_list = current->next;
        free(current);
        current = *projectile_list;
    } else {
        prev->next = current->next;
        free(current);
        current = prev->next;
    }
    return;
}

void remove_out_of_bounds_projectiles(projectile_t **projectile_list,
    float screen_width, float screen_height)
{
    projectile_t *current = *projectile_list;
    projectile_t *prev = NULL;

    while (current != NULL) {
        if (current->x < 0 || current->x > screen_width
            || current->y < 0 || current->y > screen_height) {
            handle_projectile_list(projectile_list, current, prev);
        } else {
            prev = current;
            current = current->next;
        }
    }
    return;
}

void move_projectiles(projectile_t *projectile_list, float delta_time)
{
    projectile_t *current = projectile_list;

    while (current != NULL) {
        if (current->is_alive) {
            current->x += current->dir.x * current->speed * delta_time;
            current->y += current->dir.y * current->speed * delta_time;
        }
        current = current->next;
    }
    return;
}

static void free_project_list(projectile_t **projectile_list,
    projectile_t *current, projectile_t *prev)
{
    if (prev == NULL) {
        *projectile_list = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
    return;
}

void handle_projectile_collision(projectile_t **projectile_list)
{
    projectile_t *current = *projectile_list;
    projectile_t *prev = NULL;

    while (current != NULL) {
        if (current->is_alive) {
            current->is_alive = 0;
            free_project_list(projectile_list, current, prev);
        }
        prev = current;
        current = current->next;
    }
    return;
}

void player_shoot(player_t *player, projectile_t **projectile_list)
{
    project_info_t info = {0};

    info.x = player->x;
    info.y = player->y;
    info.dir.x = cos(player->angle);
    info.dir.y = sin(player->angle);
    info.speed = 5.0f;
    info.damage = 10;
    add_projectile(projectile_list, info);
    return;
}
