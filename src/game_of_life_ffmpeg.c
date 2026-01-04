#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <raylib.h>
#include "pixelbuffer.h"

#define READ_END 0
#define WRITE_END 1

#define WIDTH 800
#define HEIGHT 600

// hack that returns the value of the argument of macro as a string
// STR(FPS) would return 60 now
#define STR2(x) #x
#define STR(x) STR2(x)

uint32_t pixels[WIDTH*HEIGHT];

unsigned int val(Color color){
    if(ColorToInt(color) == ColorToInt(WHITE)) {
        return 1;
    }
    else return 0;
}

void initialize_pixelbuf(PixelBuf* pixbuf, float threshold)
{
    for (int y = 1; y < pixbuf->height - 1; ++y) {
        for (int x = 1; x < pixbuf->width - 1; ++x) {
            // time as seed
            float rand_number = rand() / (float) RAND_MAX;
            pixbuf->pixels[y * pixbuf->width + x] = (rand_number > threshold) ? BLACK : WHITE;
        }
    }
}

Color is_alive(PixelBuf* old_pixbuf, size_t x, size_t y)
{
    size_t w = old_pixbuf->width;
    unsigned int sum =   val(old_pixbuf->pixels[(y - 1)*w + (x - 1)])
              + val(old_pixbuf->pixels[(y - 1)*w + x])
              + val(old_pixbuf->pixels[(y - 1)*w + (x + 1)])
              + val(old_pixbuf->pixels[ y*w      + (x - 1)])
              + val(old_pixbuf->pixels[ y*w      + (x + 1)])
              + val(old_pixbuf->pixels[(y + 1)*w + (x - 1)])
              + val(old_pixbuf->pixels[(y + 1)*w + x])
              + val(old_pixbuf->pixels[(y + 1)*w + (x + 1)]);

    if(sum == 2){
        return old_pixbuf->pixels[y*w + x]; // old value
    } else if (sum == 3){
        return WHITE;
    } else {
        return BLACK;
    }
}

void set_cell_states(PixelBuf* old_pixbuf, PixelBuf* new_pixbuf)
{
    for (size_t y = 1; y < old_pixbuf->height - 1; ++y) {
        for (size_t x = 1; x < old_pixbuf->width - 1; ++x) {
            new_pixbuf->pixels[y*new_pixbuf->width + x] = is_alive(old_pixbuf, x, y);
        }
    }
}

int main (void)
{
  int pipefd[2];

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
    // replace standard input of ffmpeg with the read end of the pipe (pipefd[0])
    // dup2(pipefd[READ_END], STDIN_FILENO);
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

  size_t duration = 10;

  PixelBuf* pixelbuffer[2] = {0};
  pixelbuffer[0] = create_pixelbuf(WIDTH, HEIGHT);
  pixelbuffer[1] = create_pixelbuf(WIDTH, HEIGHT);

  fill_pixels(pixelbuffer[0], BLACK);
  fill_pixels(pixelbuffer[1], BLACK);

  size_t pix_buf_idx = 0;
  initialize_pixelbuf(pixelbuffer[0], 0.3f);

  InitWindow(WIDTH, HEIGHT, "Game Of Life");
  SetTargetFPS(10);

  for (size_t i = 0; i < FPS*duration && !WindowShouldClose(); ++i) {

      set_cell_states(pixelbuffer[pix_buf_idx], pixelbuffer[(pix_buf_idx == 0)]);

      Image img = {
          .data = pixelbuffer[pix_buf_idx]->pixels,
          .width = WIDTH,
          .height = HEIGHT,
          .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
          .mipmaps = 1
      };

      Texture2D texture = LoadTextureFromImage(img);

      BeginDrawing();

      ClearBackground(*(Color*)(uint32_t[1]){0xFF181818});
      DrawTexture(texture, 0, 0, WHITE);

      EndDrawing();

      Image image = LoadImageFromTexture(texture);
      pix_buf_idx = (pix_buf_idx == 1) ? 0 : 1;

      // flip image -> writing rows in reversed order
      for (size_t y = HEIGHT; y > 0; --y) {
          write(pipefd[WRITE_END], (uint32_t*)image.data + (y - 1)*WIDTH, sizeof(uint32_t)*WIDTH);
      }

      UnloadImage(image);
  }
  CloseWindow();

  close(pipefd[WRITE_END]);

  //waiting for child being finished with execution
  wait(NULL);
  printf("Done rendering the video\n");
  return 0;
}
