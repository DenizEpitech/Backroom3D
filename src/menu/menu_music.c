/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_music
*/

#include "wolf.h"

int music(sfMusic **play_music)
{
    char *file_path = "ressources/song/backroom_song.ogg";

    *play_music = sfMusic_createFromFile(file_path);
    if (!*play_music) {
        return EXIT_ERROR;
    }
    sfMusic_setLoop(*play_music, sfTrue);
    sfMusic_play(*play_music);
    return EXIT_SUCCESS;
}
