#include "game_of_life.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

void game_loop(int fps, size_t width, size_t height, int pipe_write, size_t start_video){
    size_t frame_count = 0;
    size_t duration = 100;

    PixelBuf* pixelbuffer[2] = {0};
    pixelbuffer[0] = create_pixelbuf(width, height);
    pixelbuffer[1] = create_pixelbuf(width, height);

    fill_pixels(pixelbuffer[0], BLACK);
    fill_pixels(pixelbuffer[1], BLACK);

    size_t pix_buf_idx = 0;
    initialize_pixelbuf(pixelbuffer[0], 0.3f);

    InitWindow(width, height, "Game Of Life");
    SetTargetFPS(fps);

    for (size_t i = 0; i < fps*duration && !WindowShouldClose(); ++i) {

        set_cell_states(pixelbuffer[pix_buf_idx], pixelbuffer[(pix_buf_idx == 0)]);

        Image img = {
            .data = pixelbuffer[pix_buf_idx]->pixels,
            .width = width,
            .height = height,
            .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
            .mipmaps = 1
        };

        Texture2D texture = LoadTextureFromImage(img);

        BeginDrawing();

        ClearBackground(*(Color*)(uint32_t[1]){0xFF181818});
        DrawTexture(texture, 0, 0, WHITE);

        EndDrawing();

        pix_buf_idx = (pix_buf_idx == 1) ? 0 : 1;

        if(frame_count > start_video) {
            // flip image -> writing rows in reversed order
            for (size_t y = height; y > 0; --y) {
                write(pipe_write, (uint32_t*)img.data + (y - 1)*width, sizeof(uint32_t)*width);
            }
        }
        frame_count++;
    }
    CloseWindow();
}
