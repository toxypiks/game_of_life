#include <ctype.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "game_of_life.h"
#include "pixelbuffer.h"

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

#define WIDTH 800
#define HEIGHT 600
#define FPS 10

// hack that returns the value of the argument of macro as a string
#define STR2(x) #x
#define STR(x) STR2(x)

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

    int pipefd[2];

    if(video_flag){
        if (pipe(pipefd) < 0) {
            fprintf(stderr, "ERROR: could not create pipe: %s\n", strerror(errno));
            return 1;
        }
        pid_t child = fork();
        if (child < 0) {
            fprintf(stderr, "ERROR: could not fork a child: %s\n", strerror(errno));
            return 1;
        }
        if (child == 0) {
            // ffmpeg
            // replace standard input of ffmpeg with the read end of the pipe (pipefd[0])

            if (dup2(pipefd[READ_END], STDIN_FILENO) < 0) {
                fprintf(stderr, "ERROR: could not reopen read end of pipe as stdin: %s\n", strerror(errno));
                return 1;
            }
            close(pipefd[WRITE_END]);
            int ret = execlp("ffmpeg",
                             "ffmpeg",
                             "-loglevel", "verbose",
                             "-y",
                             "-f", "rawvideo",
                             "-pix_fmt", "rgba",
                             "-s", STR(WIDTH) "x" STR(HEIGHT),
                             "-r", STR(FPS),
                             "-an",
                             "-i", "-",
                             "-c:v", "libx264",
                             "../video_output/game_of_life.mp4",
                             NULL
            );
            if (ret < 0) {
                fprintf(stderr, "ERROR: could not run ffmpeg as a child process: %s\n", strerror(errno));
                return 1;
            }
            assert(0 && "unreachable");
        }
        close(pipefd[READ_END]);
        game_loop(FPS, WIDTH, HEIGHT, true, duration, pipefd[WRITE_END], 100);
        close(pipefd[WRITE_END]);
        //waiting for child being finished with execution
        wait(NULL);
        printf("Done rendering the video\n");
    } else { // novideo stuff
        game_loop(FPS, WIDTH, HEIGHT, false, duration ,0, 0);
    }

    return 0;
}
