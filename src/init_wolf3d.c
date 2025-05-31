/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** init_wolf3d
*/

#include "wolf.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void handle_game_events(game_t *game)
{
    while (sfRenderWindow_pollEvent(game->window, &game->event)) {
        handle_generic_events(game->window, game->event, &game->state);
        if (game->state == MENU) {
            handle_menu_events(game->window, game->event,
                game->menu, &game->state);
        }
    }
    return;
}

static void render_game_state(game_t *game)
{
    sfRenderWindow_clear(game->window, sfBlack);
    switch (game->state) {
        case MENU:
            display_menu(game->window, game->menu);
            if (game->fps_info)
                draw_fps_display(game->window, game->fps_info);
            break;
        case GAME:
            start_game(game->window, &game->state, game->fps_info);
            break;
        case SETTINGS:
            display_settings(&game->window, game->event,
                &game->state, game->menu->music);
            break;
        case EXIT:
            sfRenderWindow_close(game->window);
            break;
    }
    sfRenderWindow_display(game->window);
}

void game_loop(game_t *game)
{
    while (sfRenderWindow_isOpen(game->window)) {
        if (game->fps_info) {
            update_fps_display(game->fps_info);
        }
        handle_game_events(game);
        render_game_state(game);
    }
    return;
}

static game_t *init_game_struct(void)
{
    game_t *game = malloc(sizeof(game_t));

    if (!game)
        return NULL;
    game->window = NULL;
    game->state = MENU;
    game->menu = NULL;
    game->test_clock = NULL;
    game->clock_text = NULL;
    game->font = NULL;
    game->fps_info = NULL;
    game->map_path = NULL;
    return game;
}

static int init_game_window(game_t *game)
{
    game->window = create_window_util(1920, 1080, "WOLFENSTEIN3D",
        sfResize | sfClose);
    if (!game->window)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

static int init_game_resources(game_t *game)
{
    game->menu = create_menu();
    if (!game->menu)
        return EXIT_ERROR;
    game->fps_info = create_fps_display();
    if (!game->fps_info)
        return EXIT_ERROR;
    return EXIT_SUCCESS;
}

static void free_game_resources(game_t *game)
{
    if (!game)
        return;
    if (game->menu)
        destroy_menu(game->menu);
    if (game->window)
        sfRenderWindow_destroy(game->window);
    if (game->fps_info)
        destroy_fps_display(game->fps_info);
    if (game->map_path)
        free(game->map_path);
    free(game);
    return;
}

static int verify_map(int argc, char **argv)
{
    const char *map_path = NULL;
    game_data_t test_data = {0};
    player_t player = {0};

    if (argc < 2) {
        fprintf(stderr, "Error: No map file provided\n");
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return EXIT_ERROR;
    }
    map_path = argv[1];
    if (load_map_from_file(map_path, &test_data, &player) != EXIT_SUCCESS) {
        fprintf(stderr, "Error: Failed to load map file: %s\n", map_path);
        free_game_data(&test_data);
        return EXIT_ERROR;
    }
    free_game_data(&test_data);
    return EXIT_SUCCESS;
}

static int initialize_game(int argc, char **argv, game_t **game_ptr)
{
    game_t *game = NULL;
    int status = 0;

    status = verify_map(argc, argv);
    if (status != EXIT_SUCCESS)
        return status;
    game = init_game_struct();
    if (!game)
        return EXIT_ERROR;
    if (init_game_window(game) != EXIT_SUCCESS) {
        free_game_resources(game);
        return EXIT_ERROR;
    }
    if (init_game_resources(game) != EXIT_SUCCESS) {
        free_game_resources(game);
        return EXIT_ERROR;
    }
    game->map_path = strdup(argv[1]);
    *game_ptr = game;
    return EXIT_SUCCESS;
}

int init_wolf3d(int argc, char **argv)
{
    game_t *game = NULL;
    int status = 0;

    status = initialize_game(argc, argv, &game);
    if (status != EXIT_SUCCESS)
        return status;
    game_loop(game);
    free_game_resources(game);
    return EXIT_SUCCESS;
}
