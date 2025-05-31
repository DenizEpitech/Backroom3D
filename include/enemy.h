/*
** EPITECH PROJECT, 2025
** wold3d
** File description:
** enemy
*/

#ifndef ENEMY_H_
    #define ENEMY_H_
    #include <SFML/Graphics.h>
    #include <stdbool.h>
    #define ENEMY_PATROL_SPEED 2.0f
    #define ENEMY_CHASE_SPEED 4.0f
    #define ENEMY_ATTACK_SPEED 1.0f
    #define ENEMY_HURT_RECOIL 6.0f
    #define ENEMY_DIR_CHANGE_TIME 3.0f
    #define ENEMY_SHOOT_COOLDOWN 1.2f
    #define DEFAULT_DETECTION_RANGE 250.0f
    #define DEFAULT_ATTACK_RANGE 100.0f
    #define DEFAULT_PATROL_RADIUS (3 * TILE_SIZE)
    #define MIN_ENEMIES_PER_MAP 3
    #define MAX_ENEMIES_PER_MAP 8
    #define MIN_DISTANCE_FROM_PLAYER 5.0f
    #define MIN_DISTANCE_BETWEEN_ENEMIES 3.0f
    #define MAX_SPAWN_ATTEMPTS 100

typedef struct game_data_s game_data_t;
typedef struct player_s player_t;
typedef struct projectile_t projectile_t;

typedef enum {
    ENEMY_TYPE_SOLDIER,
} enemy_type_t;

typedef enum {
    ENEMY_STATE_IDLE,
    ENEMY_STATE_PATROL,
    ENEMY_STATE_CHASE,
    ENEMY_STATE_ATTACK,
    ENEMY_STATE_HURT,
    ENEMY_STATE_DYING,
    ENEMY_STATE_DEAD
} enemy_state_t;

typedef enum {
    DIRECTION_FRONT,
    DIRECTION_BACK,
    DIRECTION_RIGHT,
    DIRECTION_LEFT,
    DIRECTION_RIGHT_FRONT,
    DIRECTION_LEFT_FRONT,
    DIRECTION_RIGHT_BACK,
    DIRECTION_LEFT_BACK
} enemy_direction_t;

typedef struct animation_params_s {
    enemy_state_t state;
    enemy_direction_t direction;
    int frame_count;
    float frame_time;
} animation_params_t;

typedef struct animation_setup_s {
    enemy_state_t state;
    enemy_direction_t direction;
    int frame_count;
    float frame_time;
} animation_setup_t;

typedef struct animation_config_s {
    int idle_frames;
    int patrol_frames;
    int chase_frames;
    int attack_frames;
    int dying_frames;
    float idle_time;
    float patrol_time;
    float chase_time;
    float attack_time;
    float dying_time;
} animation_config_t;

typedef struct spawn_candidate_t {
    int x;
    int y;
    float world_x;
    float world_y;
} spawn_candidate_t;

typedef struct enemy_animation_frame_s {
    sfTexture *texture;
    sfSprite *sprite;
} enemy_animation_frame_t;

typedef struct enemy_animation_set_s {
    enemy_animation_frame_t *frames;
    int frame_count;
    int current_frame;
    float frame_time;
    float elapsed_time;
    sfClock *animation_clock;
} enemy_animation_set_t;

typedef struct enemy_animations_s {
    enemy_animation_set_t idle;
    enemy_animation_set_t patrol;
    enemy_animation_set_t chase;
    enemy_animation_set_t attack;
    enemy_animation_set_t hurt;
    enemy_animation_set_t dying;
    enemy_animation_set_t *current;
} enemy_animations_t;

typedef struct enemy_stats_s {
    int health;
    int max_health;
    float speed;
    int damage;
    float detection_range;
    float attack_range;
} enemy_stats_t;

typedef struct patrol_info_s {
    float start_x;
    float start_y;
    float patrol_radius;
    float current_angle;
    float patrol_speed;
} patrol_info_t;

typedef struct enemy_s {
    float x;
    float y;
    float angle;
    enemy_type_t type;
    enemy_state_t state;
    enemy_state_t previous_state;
    enemy_direction_t direction;
    enemy_stats_t stats;
    enemy_animations_t animations;
    struct enemy_s *next;
    game_data_t *data;
    patrol_info_t patrol;
} enemy_t;

typedef struct enemy_render_data_s {
    enemy_t *enemy;
    float distance;
    float angle;
    float screen_x;
    float size;
    int visible;
} enemy_render_data_t;

typedef struct target_params_s {
    sfVector2i crosshair;
    sfVector2u win_size;
    float tolerance;
} target_params_t;

void init_patrol_settings(enemy_t *enemy);
int initialize_enemy_components(enemy_t *enemy);
enemy_t *create_enemy(float x, float y);
enemy_t *create_enemy_with_type(float x, float y, enemy_type_t type);
void destroy_enemy(enemy_t *enemy);
void add_enemy_to_list(enemy_t **list, enemy_t *enemy);
void destroy_all_enemies(enemy_t *enemy_list);

enemy_t *allocate_enemy(void);
void init_random_seed(void);
const char *get_enemy_texture_path(enemy_type_t type);
void init_enemy_stats(enemy_stats_t *stats, enemy_type_t type);

int can_move_to_position(game_data_t *game_data, float x, float y);
void scan_map_for_enemies(game_data_t *game_data);
void spawn_random_enemies(game_data_t *game_data, player_t *player);
float calculate_enemy_distance(enemy_t *enemy, player_t *player);
float calculate_enemy_angle(enemy_t *enemy, player_t *player);
int is_enemy_visible(enemy_t *enemy, player_t *player, game_data_t *game_data);
void render_all_enemies(sfRenderWindow *window, enemy_t *enemy_list,
    player_t *player, game_data_t *game_data);

bool is_valid_target(enemy_t *enemy, player_t *player,
    target_params_t *params);
void update_best_target(enemy_t *enemy, player_t *player,
    enemy_t **best_target, float *best_distance);
sfVector2i get_crosshair_position(sfRenderWindow *window);

int init_enemy_animations(enemy_t *enemy);
void update_enemy_animation(enemy_t *enemy, float delta_time);
void set_enemy_animation_state(enemy_t *enemy, enemy_state_t new_state);
void cleanup_enemy_animations(enemy_animations_t *animations);
enemy_direction_t calculate_enemy_direction(enemy_t *enemy, player_t *player);

void update_enemy_behavior(enemy_t *enemy, player_t *player, float delta_time);
void enemy_shoot_at_player(enemy_t *enemy, player_t *player,
    projectile_t **projectile_list);
void update_enemy_health(enemy_t *enemy, int damage);

animation_config_t get_animation_config(void);
int init_basic_animations(enemy_t *enemy, animation_config_t config);
int init_combat_animations(enemy_t *enemy, animation_config_t config);

void cleanup_partial_frames(enemy_animation_frame_t *frames, int count);
int load_frame_texture(enemy_animation_frame_t *frame,
    animation_params_t params, int i);
int create_frame_sprite(enemy_animation_frame_t *frame);

void update_enemy_pursuit(enemy_t *enemy, player_t *player,
    float delta_time);
void update_enemy_patrol(enemy_t *enemy, float delta_time);

#endif
