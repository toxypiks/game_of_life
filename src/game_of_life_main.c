#include <ctype.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "game_of_life.h"
#include "pixelbuffer.h"


int main (int argc, char **argv)
{
    int video_flag = 0;
    char *duration_value = NULL;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "vd:")) != -1) {
        switch (c)
        {
            case 'v':
                video_flag = 1;
                break;
            case 'd':
                duration_value = optarg;
                break;
            case '?':
                printf("TODO: how to use me\n");
                break;
            default:
                abort ();
        }
    }
    // convert ascii to int
    int duration = -1;
    if(duration_value) {
        duration = atoi(duration_value);
        printf("cli arguments: -v: %d, -d: time:%d\n",video_flag, duration);
    } else {
        printf("cli arguments: -v: %d\n",video_flag);
    }
    game_loop(10, 400, 400, false, 0, 0, 0);
    return 0;
}
