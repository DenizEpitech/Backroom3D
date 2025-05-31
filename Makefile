##
## EPITECH PROJECT, 2025
## G-ING-210-PAR-2-1-wolf3d
## File description:
## Makefile
##

CC      ?= gcc
RM      ?= rm -f
FIND    = find
ECHO    = echo -e

SRC_DIR     = src
INCLUDE_DIR = include

NAME        = wolf3d

BLACK       = \033[0;30m
RED         = \033[0;31m
GREEN       = \033[0;32m
YELLOW      = \033[0;33m
BLUE        = \033[0;34m
MAGENTA     = \033[0;35m
CYAN        = \033[0;36m
WHITE       = \033[0;37m
BOLD        = \033[1m
RESET       = \033[0m

CFLAGS      ?= -Wall -Wextra -iquote $(INCLUDE_DIR)
LDFLAGS     ?= -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lm

DEBUG_FLAGS    ?= -g3 -DDEBUG

SRC_CORE = display_window.c fps.c init_wolf3d.c init_view.c \
           game_core.c game_loop.c game_events.c display_level_state.c \
		   render_elements.c \

SRC_MENU = menu/menu.c menu/menu_manager.c menu/button.c menu/menu_music.c \
		   menu/setting/setting_main.c menu/setting/setting_ui.c \
		   menu/setting/setting_events.c menu/setting/setting_volume.c \
		   menu/utils.c

SRC_ENGINE = look/initialize_engine.c look/player_movement.c \
             look/raycasting_implementation.c look/wall_projection.c \
             look/textures.c look/texture_sprites.c look/wall_renderer.c \
			 look/raycasting_core.c look/floor_ceiling_rendering.c \
			 look/flashlight.c \

SRC_PARSING = parsing/free_map.c parsing/map_dimension.c parsing/map_memory.c \
              parsing/map_reader.c parsing/map_loader.c \
			  parsing/map_character.c parsing/save_map.c \

SRC_WEAPON = fight/weapon.c fight/weapon_animation.c \
			 fight/weapon_animation_render.c fight/manage_projectile.c \
			 fight/crosshair_animation.c fight/manage_shoot.c \
			 fight/health_animation.c fight/manage_health.c \
			 fight/init_health.c

SRC_ENEMY = enemy/enemy.c enemy/enemy_manager.c enemy/map_enemy.c \
            enemy/enemy_behavior.c enemy/enemy_movement.c enemy/enemy_calc.c \
            enemy/init_ennemi.c enemy/enemy_states.c \
            enemy/animation/enemy_animation.c \
            enemy/animation/animation_init.c \
            enemy/animation/animation_loader.c enemy/enemy_render.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_CORE) \
    $(SRC_MENU) $(SRC_ENGINE) $(SRC_PARSING) $(SRC_WEAPON) $(SRC_ENEMY))

MAIN = main.c

OBJ         = $(SRC:.c=.o)
MAIN_OBJ    = $(MAIN:.c=.o)

all: intro $(NAME) outro

intro:
	@clear
	@$(ECHO) "$(RED)  ██╗    ██╗ ██████╗ ██╗     ███████╗██████╗ ██████╗"
	@$(ECHO) "$(RED)  ██║    ██║██╔═══██╗██║     ██╔════╝╚════██╗██╔══██╗"
	@$(ECHO) "$(RED)  ██║ █╗ ██║██║   ██║██║     █████╗   █████╔╝██║  ██║"
	@$(ECHO) "$(RED)  ██║███╗██║██║   ██║██║     ██╔══╝   ╚═══██╗██║  ██║"
	@$(ECHO) "$(RED)  ╚███╔███╔╝╚██████╔╝███████╗██║     ██████╔╝██████╔╝"
	@$(ECHO) "$(RED)   ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     ╚═════╝ ╚═════╝"
	@$(ECHO) "$(YELLOW)===============================================$(RESET)"
	@$(ECHO) "$(GREEN)➤ Building Wolf3D project$(RESET)"
	@$(ECHO) "$(YELLOW)===============================================$(RESET)"

outro:
	@$(ECHO) "$(GREEN)$(BOLD)  🎮 WOLF3D successfully built! 🎮$(RESET)"
	@$(ECHO) "$(CYAN)  Run with: $(WHITE)./$(NAME) map/[map_file]$(RESET)"

$(NAME): $(MAIN_OBJ) $(OBJ)
	@$(ECHO) "$(YELLOW)⚡ Linking executable: $(CYAN)$@$(RESET)"
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@$(ECHO) "$(GREEN)✓ Build complete!$(RESET)"

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean intro $(NAME)
	@$(ECHO) "$(GREEN)✓ Debug build complete!$(RESET)"

coverage: CFLAGS += --coverage $(DEBUG_FLAGS)
coverage: LDFLAGS += --coverage
coverage: clean
	@$(ECHO) "$(YELLOW)Building with coverage instrumentation...$(RESET)"
	@$(MAKE) -s $(NAME)
	@$(ECHO) "$(GREEN)✓ Coverage build complete!$(RESET)"

tests_run: coverage
	@$(ECHO) "$(YELLOW)Running unit tests...$(RESET)"
	@$(ECHO) "$(YELLOW)No unit tests implemented yet$(RESET)"
	@gcovr --exclude tests/ -s || true
	@$(ECHO) "$(GREEN)✓ Tests completed!$(RESET)"

%.o: %.c
	@$(ECHO) "$(BLUE)Compiling: $(WHITE)$<$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(ECHO) "$(RED)🧹 Cleaning object files...$(RESET)"
	@$(FIND) . -name "*.o" -delete
	@$(ECHO) "$(GREEN)✓ Object files removed.$(RESET)"

fclean: clean
	@$(ECHO) "$(RED)╔══════════════════════════════════════╗$(RESET)"
	@$(ECHO) "$(RED)║         FULL CLEANUP WIZARD          ║$(RESET)"
	@$(ECHO) "$(RED)╚══════════════════════════════════════╝$(RESET)"
	@$(ECHO) "$(YELLOW)➤ $(CYAN)Removing coverage files...$(RESET)"
	@$(ECHO) "$(YELLOW)➤ $(CYAN)Removing executable and debug files...$(RESET)"
	@$(RM) $(NAME) a.out vgcore*
	@$(ECHO) "$(GREEN)╔══════════════════════════════════════╗$(RESET)"
	@$(ECHO) "$(GREEN)║       CLEANUP COMPLETE! (100%)       ║$(RESET)"
	@$(ECHO) "$(GREEN)╚══════════════════════════════════════╝$(RESET)"

re: fclean all

.PHONY: make all debug coverage tests_run clean fclean re help intro outro
