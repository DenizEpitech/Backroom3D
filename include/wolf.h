/*
** EPITECH PROJECT, 2025
** G-ING-210-PAR-2-1-wolf3d
** File description:
** wolf.h
*/

#ifndef WOLF_H_
    #define WOLF_H_
    #define EXIT_ERROR 84
    #define EXIT_SUCCESS 0
    #define FUNCTION_ERROR -1
    #define TRUE 1
    #define FALSE 0
    #define WIDTH 1920
    #define HEIGHT 1080
    #define TILE_SIZE 64
    #define MAP_WIDTH 8
    #define MAP_HEIGHT 8
    #define FOV (M_PI / 3)
    #define NUM_RAYS 1920
    #define WINDOW_WIDTH 1920
    #define WINDOW_HEIGHT 1080
    #define MAX_TEXTURES 10
    #define GAME_NEXT_LEVEL 1
    #define GAME_EXIT 2
    #define PLAYER_MOVE_SPEED 5.0f
    #define PLAYER_ROTATION_SPEED 0.03f
    #define SPEED_MULTIPLIER 2.0f
    #include <SFML/Graphics.h>
    #include <SFML/Audio.h>
    #include <math.h>
    #include <float.h>
    #include "enemy.h"
    #include "menu.h"

typedef struct weapon_s weapon_t;
typedef struct minimap_s minimap_t;
typedef struct crosshair_s {
    sfRectangleShape *horizontal;
    sfRectangleShape *vertical;
} crosshair_t;

typedef struct wall_hit_s {
    float distance;
    int map_x;
    int map_y;
    float texture_x;
    int side;
    int map_value;
} wall_hit_t;

typedef struct dda_algo_s {
    float ray_angle;
    float ray_dir_x;
    float ray_dir_y;
    int step_x;
    int step_y;
    int map_x;
    int map_y;
    float pos_x;
    float pos_y;
    float delta_dist_x;
    float delta_dist_y;
} dda_algo_t;

typedef struct texture_manager_s {
    sfTexture *wall_textures[MAX_TEXTURES];
    sfSprite *wall_sprites[MAX_TEXTURES];
    sfTexture *floor_texture;
    sfSprite *floor_sprite;
    sfTexture *ceiling_texture;
    sfSprite *ceiling_sprite;
} texture_manager_t;

typedef struct health_sprite_system_s {
    sfSprite **health_sprites;
    int max_health;
    int current_health;
    int is_visible;
    float display_timer;
    int current_sprite_index;
    sfVector2f position;
    sfTexture **health_textures;
} health_sprite_system_t;

typedef struct projectile_t {
    float x;
    float y;
    sfVector2f dir;
    float speed;
    int damage;
    int is_alive;
    struct projectile_t *next;
} projectile_t;

typedef struct project_info_s {
    float x;
    float y;
    sfVector2f dir;
    float speed;
    int damage;
} project_info_t;

typedef struct game_data_s {
    int **map;
    int map_width;
    int map_height;
    projectile_t *projectiles;
    struct player_s *player;
    texture_manager_t *texture_manager;
    enemy_t *enemies;
    int enemy_count;
    health_sprite_system_t *health_system;
} game_data_t;

typedef struct game_context_s {
    sfRenderWindow *window;
    game_state_t *state;
    fps_info_t *fps_info;
    char *map_path;
    sfEvent event;
    game_data_t game_data;
    sfSound *shoot_sound;
    sfSoundBuffer *shoot_buffer;
    sfView *game_view;
    sfRenderTexture *light_texture;
    sfSprite *light_sprite;
    bool light_state;
    crosshair_t *crosshair;
    health_sprite_system_t *health_system;
} game_context_t;

typedef struct shoot_effect_s {
    float alpha;
    float cooldown;
    sfSound *sound;
    sfSoundBuffer *buffer;
    sfVector2i position;
} shoot_effect_t;

typedef struct player_s {
    float x;
    float y;
    float angle;
    game_context_t *context;
    game_data_t *data;
    float shoot_timer;
    int is_shooting;
    shoot_effect_t shoot;
} player_t;

typedef struct init_context_params_s {
    sfRenderWindow *window;
    game_state_t *state;
    fps_info_t *fps_info;
    char *map_path;
    player_t *player;
} init_context_params_t;

typedef struct dda_context_s {
    float side_dist_x;
    float side_dist_y;
    int side;
    int steps;
    int max_steps;
} dda_context_t;

typedef struct hit_calculation_params_s {
    float distance;
    float corrected_distance;
    float wall_x;
} hit_calculation_params_t;

typedef struct event_context_s {
    sfEvent *event;
    game_state_t *state;
    game_context_t *ctx;
    player_t *player;
    weapon_t *weapon;
} event_context_t;

typedef struct game_update_context_s {
    game_context_t *ctx;
    player_t *player;
    weapon_t *weapon;
    float delta_time;
} game_update_context_t;

typedef struct game_loop_resources_s {
    weapon_t *weapon;
    sfClock *delta_clock;
    sfTime last_frame;
    event_context_t event_ctx;
    game_update_context_t update_ctx;
} game_loop_resources_t;

typedef struct level_ctx_s {
    sfRenderWindow *window;
    const char *map_path;
    game_state_t *state;
    fps_info_t *fps_info;
    game_context_t *ctx;
    player_t *player;
} level_ctx_t;

typedef struct sprint_state_s {
    int is_sprint_active;
} sprint_state_t;

health_sprite_system_t *init_health_sprite_system(sfSprite **sprites,
    int max_health);
sfSprite *load_health_sprite(const char *path);
void update_health_sprite_system(health_sprite_system_t *system,
    player_t *player, enemy_t *enemy_list, float delta_time);
void render_health_sprite(sfRenderWindow *window,
    health_sprite_system_t *system);
void set_health_sprite_position(health_sprite_system_t *system,
    float x, float y);
int is_player_dead(health_sprite_system_t *system);
void apply_damage_to_player(health_sprite_system_t *system, int damage);
void heal_player(health_sprite_system_t *system, int heal_amount);
void destroy_health_sprite_system(health_sprite_system_t *system);
void update_health_sprite_system_simple(health_sprite_system_t *system,
    player_t *player, enemy_t *enemy_list, float delta_time);
int calculate_health_sprite_index(float health_percentage);
void handle_damage_and_display(health_sprite_system_t *system,
    int player_under_attack, float *damage_cooldown);
void update_display_and_sprite(health_sprite_system_t *system);
int is_enemy_nearby(enemy_t *enemy_list, player_t *player);
int setup_health_system(game_context_t *ctx);

void close_window(sfRenderWindow *window, sfEvent event);
int music(sfMusic **play_music);
int init_wolf3d(int argc, char **argv);

int display_settings(sfRenderWindow **window, sfEvent event,
    game_state_t *state, sfMusic *menu_music);

int load_map_from_file(const char *filename, game_data_t *game_data,
    player_t *player);
void free_game_data(game_data_t *game_data);

void init_player(player_t *player);
void update_player(player_t *player);
int is_wall(game_data_t *game_data, int x, int y);
void draw_floor_and_celling(sfRenderWindow *window);
void render_wall_column(sfRenderWindow *window, int column,
    float distance, sfColor color);
level_ctx_t init_level(sfRenderWindow *window, game_state_t *state,
    fps_info_t *fps_info, game_context_t *ctx);
int start_game(sfRenderWindow *window, game_state_t *state,
    fps_info_t *fps_info);
void cast_all_rays(sfRenderWindow *window, game_data_t *game_data);
sfColor chose_colors(game_data_t *game_data, int map_x, int map_y);
wall_hit_t *cast_single_ray(game_data_t *game_data, float ray_angle);
void cleanup_partial_allocation(int **map, int allocated_rows);

texture_manager_t *init_texture_manager(void);
void destroy_texture_manager(texture_manager_t *manager);
void draw_textured_floor_and_ceiling(sfRenderWindow *window,
    texture_manager_t *texture_manager);
void render_textured_wall_column(sfRenderWindow *window, int column,
    wall_hit_t *hit, texture_manager_t *texture_manager);

void play_shoot_sound(sfSound* sound);
void player_shoot_at_position(player_t *player,
    projectile_t **projectile_list, sfVector2i mousePos);
shoot_effect_t init_shoot_effect(void);
void init_shoot_sound(sfSound **sound, sfSoundBuffer **buffer);
void add_projectile(projectile_t **head, project_info_t info);
void update_shoot_effect(shoot_effect_t *shoot);
void init_game_view(game_context_t *ctx,
    init_context_params_t params);
int save_file(game_context_t *ctx);

int run_game_loop(game_context_t *ctx, player_t *player);
event_context_t init_event_context(game_context_t *ctx, weapon_t *weapon);
game_update_context_t init_update_context(game_context_t *ctx,
    player_t *player, weapon_t *weapon);
void analyse_events(event_context_t *context);
void manage_resize_window(weapon_t *weapon, sfEvent *event,
    game_context_t *ctx);
void display_completed_level(sfRenderWindow *window);
void init_game_components(game_context_t *ctx);
void cleanup_game_components(game_context_t *ctx);
void update_crosshair(crosshair_t *crosshair, sfRenderWindow *window);
void draw_crosshair(crosshair_t *crosshair, sfRenderWindow *window);

void enemy_shoot_at_player(enemy_t *enemy, player_t *player,
    projectile_t **projectile_list);
void update_enemy_behavior(enemy_t *enemy, player_t *player, float delta_time);
void update_enemy_health(enemy_t *enemy, int damage);

void cleanup_game_loop_ressources(game_loop_resources_t *resources);
int init_game_loop_resources(game_loop_resources_t *resources,
    game_context_t *ctx, player_t *player);
void process_game_frame(game_loop_resources_t *resources);

void setup_menu_view(sfRenderWindow *window);
void game_loop(game_t *game);

void display_completed_level(sfRenderWindow *window);
int initialise_lightning(game_context_t *ctx);
void draw_flashlight_overlay(game_context_t *ctx, float max_radius);
void render_game_elements(game_update_context_t *context);
void display_game_completed(sfRenderWindow *window);
void manage_game_state(sfRenderWindow *window, game_state_t *state);
void display_game_over(sfRenderWindow *window);

#endif
