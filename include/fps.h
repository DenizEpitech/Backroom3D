/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** fps
*/

#ifndef FPS_H_
    #define FPS_H_
    #include <SFML/Graphics.h>

typedef struct fps_info_s {
    sfClock *fps_clock;
    sfText *fps_text;
    sfFont *font;
    float fps;
    int frame_count;
    float elapsed_time;
} fps_info_t;

fps_info_t *create_fps_display(void);
void update_fps_display(fps_info_t *fps_info);
void draw_fps_display(sfRenderWindow *window, fps_info_t *fps_info);
void destroy_fps_display(fps_info_t *fps_info);

#endif
